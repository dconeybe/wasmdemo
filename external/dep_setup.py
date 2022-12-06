from __future__ import annotations

import abc
import argparse
from collections.abc import Generator, Mapping, Sequence
import contextlib
import dataclasses
import hashlib
import io
import json
import logging
import os
import pathlib
import shutil
import sys
import tempfile
from typing import Any, IO, Optional
import zipfile

import requests


DEFAULT_CACHE_DIR = pathlib.Path.home() / ".dep_setup_cache"


@dataclasses.dataclass(frozen=True)
class OpenedFilePathPair:
  path: pathlib.Path
  f: IO[bytes]


class CacheFile(abc.ABC):

  @abc.abstractmethod
  def create(self) -> OpenedFilePathPair:
    raise NotImplementedError()

  @abc.abstractmethod
  def commit(self) -> OpenedFilePathPair:
    raise NotImplementedError()

  @abc.abstractmethod
  def rollback(self) -> None:
    raise NotImplementedError()

  @abc.abstractmethod
  def close(self) -> None:
    raise NotImplementedError()

  def __enter__(self) -> OpenedFilePathPair:
    return self.create()

  def __exit__(self, exc_type, exc_val, exc_tb) -> None:
    self.close()


class TransientCacheFile(CacheFile):

  def __init__(self, name_hint: str) -> None:
    self.name_hint = name_hint
    self.synthetic_path = pathlib.Path(tempfile.gettempdir()) / name_hint
    self.f: IO[bytes] = None

  def create(self) -> OpenedFilePathPair:
    self.f = tempfile.TemporaryFile(suffix=self.name_hint)
    return OpenedFilePathPair(path=self.synthetic_path, f=self.f)

  def commit(self) -> OpenedFilePathPair:
    self.f.seek(0)
    return OpenedFilePathPair(path=self.synthetic_path, f=self.f)

  def rollback(self) -> None:
    self.f.close()

  def close(self) -> None:
    self.f.close()
    

class PersistentCacheFile(CacheFile):

  def __init__(self, dest_file: pathlib.Path) -> None:
    self.dest_file = dest_file
    self.f: IO[bytes] = None
    self.path: pathlib.Path = None

  def create(self) -> OpenedFilePathPair:
    if not self.dest_file.parent.exists():
      self.dest_file.parent.mkdir(parents=True, exist_ok=True)
    (temp_file_handle, temp_file_path) = tempfile.mkstemp(
      f"_{self.dest_file.name}", dir=self.dest_file.parent)
    self.f = os.fdopen(temp_file_handle, 'wb')
    self.path = pathlib.Path(temp_file_path)
    return OpenedFilePathPair(path=self.path, f=self.f)

  def commit(self) -> OpenedFilePathPair:
    try:
      self.f.close()
      self.f = None
      logging.info("Renaming %s to %s", self.path, self.dest_file.name)
      self.path.rename(self.dest_file)
    except:
      logging.info("Deleting %s", self.path)
      self.path.unlink(missing_ok=True)
      raise

    return OpenedFilePathPair(path=self.dest_file, f=self.dest_file.open("rb"))

  def rollback(self) -> None:
    try:
      self.f.close()
    finally:
      logging.info("Deleting %s", self.path)
      self.path.unlink(missing_ok=True)

  def close(self) -> None:
    if self.f:
      self.rollback()


class Stamp:

  def __init__(self, package_name: str, package_version: str) -> None:
    self.package_name = package_name
    self.package_version = package_version

  def save(self, file_path: pathlib.Path) -> None:
    data = {
      "package_name": str(self.package_name),
      "package_version": str(self.package_version),
    }
    with file_path.open("wt", encoding="utf8") as f:
      json.dump(data, f, ensure_ascii=False, indent=2, sort_keys=True)

  def __eq__(self, other: Any) -> bool:
    try:
      other_package_name = other.package_name
      other_package_version = other.package_version
    except AttributeError:
      return False
    else:
      return (
        other_package_name == self.package_name and
        other_package_version == self.package_version
      )

  def __ne__(self, other: Any) -> bool:
    return not self.__eq__(other)

  @classmethod
  def load(cls, file_path: pathlib.Path) -> Stamp:
    with file_path.open("rt", encoding="utf8") as f:
      # Set an upper limit on the number of bytes read to avoid DoS attacks.
      # See https://docs.python.org/3.9/library/json.html for details.
      encoded_json = f.read(8192)

    decoded_json = json.loads(encoded_json)
    if not isinstance(decoded_json, Mapping):
      raise cls.StampDecodeError(f"Decoded JSON should be a map, but got {type(decoded_json)}")

    package_name = decoded_json.get("package_name")
    if package_name is None:
      raise cls.StampDecodeError("package_name key/value pair is missing")
    if not isinstance(package_name, str):
      raise cls.StampDecodeError(f"package_name should be a string, but got {type(package_name)}")

    package_version = decoded_json.get("package_version")
    if package_version is None:
      raise cls.StampDecodeError("package_version key/value pair is missing")
    if not isinstance(package_version, str):
      raise cls.StampDecodeError(f"package_version should be a string, but got {type(package_version)}")

    return cls(
      package_name=package_name,
      package_version=package_version,
    )

  class StampDecodeError(Exception):
    pass


class Downloader:

  def __init__(
    self,
    package_name: str,
    package_version: str,
    download_url: str,
    expected_sha256: str,
    dest_dir: pathlib.Path,
    cache_dir: Optional[pathlib.Path],
  ) -> None:
    self.package_name = package_name
    self.package_version = package_version
    self.download_url = download_url
    self.expected_sha256 = expected_sha256
    self.dest_dir = dest_dir
    self.cache_dir = cache_dir
    self.stamp_file = dest_dir / "dep_setup.stamp.json"
    self.stamp = Stamp(package_name=package_name, package_version=package_version)

  def run(self) -> None:
    if self.is_stamp_file_valid():
      logging.info("Stamp file %s is valid; nothing to do", self.stamp_file)
      return

    if self.dest_dir.exists():
      logging.info("Deleting %s", self.dest_dir)
      shutil.rmtree(self.dest_dir)

    with self.download() as zip_file_path_pair:
      self.verify_sha256(zip_file_path_pair)
      self.extract(zip_file_path_pair)

    self.fixup()
    self.write_stamp_file()

  def is_stamp_file_valid(self) -> bool:
    if not self.stamp_file.exists():
      return False

    logging.info("Validating stamp file: %s", self.stamp_file)
    try:
      stamp = Stamp.load(self.stamp_file)
    except (IOError, Stamp.StampDecodeError, UnicodeDecodeError, json.JSONDecodeError) as e:
      logging.info("Validating stamp file %s failed: %s", self.stamp_file, e)
      return False

    if stamp != self.stamp:
      logging.info("Validating stamp file %s failed: stamp does not equal the "
        "expected stamp", self.stamp_file)
      return False

    # Stamp file is valid.
    return True

  def write_stamp_file(self) -> bool:
    logging.info("Writing stamp to %s", self.stamp_file)
    self.stamp.save(self.stamp_file)

  @contextlib.contextmanager
  def download(self) -> Generator[OpenedFilePathPair, None, None]:
    download_dest_file_name = f"{self.package_name}_{self.package_version}.zip"
    if not self.cache_dir:
      download_cache_file = TransientCacheFile(name_hint=download_dest_file_name)
    else:
      download_dest_file = self.cache_dir / download_dest_file_name

      # Use the previously-downloaded file, if it exists
      if download_dest_file.exists():
        logging.info("Using previously-downloaded file %s", download_dest_file)
        with download_dest_file.open("rb") as f:
          yield OpenedFilePathPair(download_dest_file, f)
          return

      download_cache_file = PersistentCacheFile(download_dest_file)

    with download_cache_file as download_cache_file_file_path_pair:
      download_file_path = download_cache_file_file_path_pair.path
      download_file = download_cache_file_file_path_pair.f
      logging.info("Downloading %s to %s", self.download_url, download_file_path)

      session = requests.Session()
      response = session.get(self.download_url, stream=True)
      response.raise_for_status()
      content_length = response.headers.get("Content-Length")
      if content_length:
        try:
          content_length_int = int(content_length)
        except ValueError:
          pass
        else:
          logging.info("Downloading %s bytes", f"{content_length_int:,}")
      downloaded_bytes_count = 0
      for chunk in response.iter_content(chunk_size=None):
        download_file.write(chunk)
        downloaded_bytes_count += len(chunk)

      logging.info(
        "Downloaded %s bytes from %s",
        f"{downloaded_bytes_count:,}",
        self.download_url,
      )

      committed_file_path_pair = download_cache_file.commit()
      with committed_file_path_pair.f:
        yield committed_file_path_pair

  def verify_sha256(self, zip_file_path_pair: OpenedFilePathPair) -> None:
    logging.info("Verifying SHA256 hash of %s", zip_file_path_pair.path)
    hasher = hashlib.sha256()
    while True:
      chunk = zip_file_path_pair.f.read(65536)
      if not chunk:
        break
      hasher.update(chunk)

    zip_file_path_pair.f.seek(0)

    actual_sha256 = hasher.hexdigest()
    if actual_sha256 != self.expected_sha256:
      raise self.HashMismatch(
        f"The SHA256 hash of {zip_file_path_pair.path} was "
        f"{actual_sha256} but expected {self.expected_sha256}"
      )

  def extract(self, zip_file_path_pair: OpenedFilePathPair) -> None:
    if not self.dest_dir.exists():
      logging.info("Creating directory: %s", self.dest_dir)
      self.dest_dir.mkdir(parents=True, exist_ok=True)

    logging.info("Unzipping %s to %s", zip_file_path_pair.path, self.dest_dir)
    with zipfile.ZipFile(zip_file_path_pair.f) as zf:
      zf.extractall(self.dest_dir)

  def fixup(self) -> None:
    dest_subdir = self.dest_single_subdir()

    logging.info("Moving the contents of %s to its parent directory", dest_subdir)
    for src_file in dest_subdir.iterdir():
      dest_file = src_file.parent.parent / src_file.name
      logging.debug("Moving %s to %s", src_file, dest_file)
      src_file.rename(dest_file)

    logging.debug("Deleting empty directory: %s", dest_subdir)
    dest_subdir.rmdir()

  def dest_single_subdir(self) -> pathlib.Path:
    single_subdir = None
    for path_entry in self.dest_dir.iterdir():
      if single_subdir is None:
        single_subdir = path_entry
      else:
        raise self.InvalidDirectoryLayout(f"{self.dest_dir} contains more than "
          f"one file/directory, but expected exactly one directory; found "
          f"{single_subdir.name} and {path_entry.name}")

    if single_subdir is None:
      raise self.InvalidDirectoryLayout(f"{self.dest_dir} should contain "
        "exactly one subdirectory; however, it is completely empty")
    if not single_subdir.is_dir():
      raise self.InvalidDirectoryLayout(f"{self.dest_dir} should contain "
        f"exactly one subdirectory; however, it contained a non-directory, "
        f"{single_subdir.name}")

    return single_subdir

  class InvalidDirectoryLayout(Exception):
    pass

  class HashMismatch(Exception):
    pass


@dataclasses.dataclass(frozen=True)
class ParsedArguments:
  package_name: str
  package_version: str
  download_url: str
  sha256: str
  dest_dir: pathlib.Path
  cache_dir: Optional[pathlib.Path]


class ArgumentParseError(Exception):
  pass


def parse_arguments(args: Sequence[str]) -> ParsedArguments:
  arg_parser = argparse.ArgumentParser(
    prog=os.path.basename(sys.argv[0]),
  )

  arg_parser.add_argument("package_name")
  arg_parser.add_argument("package_version")
  arg_parser.add_argument("download_url")
  arg_parser.add_argument("sha256")
  arg_parser.add_argument("dest_dir")

  cache_dir_arg = arg_parser.add_argument(
    "--cache_dir",
    help=f"Default: {DEFAULT_CACHE_DIR}",
  )
  cache_enabled_arg = arg_parser.add_argument(
    "--cache_enabled",
    action="store_true",
    default=True,
  )
  arg_parser.add_argument(
    "--nocache_enabled",
    action="store_false",
    dest=cache_enabled_arg.dest,
  )

  parse_args_result = arg_parser.parse_args(args[1:])

  if not parse_args_result.cache_enabled:
    cache_dir = None
  elif parse_args_result.cache_dir:
    cache_dir = pathlib.Path(parse_args_result.cache_dir)
  else:
    cache_dir = DEFAULT_CACHE_DIR

  return ParsedArguments(
    package_name=parse_args_result.package_name,
    package_version=parse_args_result.package_version,
    download_url=parse_args_result.download_url,
    sha256=parse_args_result.sha256,
    dest_dir=pathlib.Path(parse_args_result.dest_dir),
    cache_dir=cache_dir,
  )


def main() -> None:
  logging.basicConfig(
    level=logging.INFO,
    format="%(filename)s %(message)s",
  )

  args = parse_arguments(sys.argv)

  downloader = Downloader(
    package_name=args.package_name,
    package_version=args.package_version,
    download_url=args.download_url,
    expected_sha256=args.sha256,
    dest_dir=args.dest_dir,
    cache_dir=args.cache_dir,
  )

  downloader.run()


if __name__ == "__main__":
  main()
