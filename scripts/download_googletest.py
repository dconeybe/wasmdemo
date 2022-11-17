"""
Syntax: %s <git_commit> <dest_dir>
"""

from __future__ import annotations

import abc
from collections.abc import Generator, Mapping, Sequence
import contextlib
import dataclasses
import io
import json
import os
import pathlib
import shutil
import tempfile
from typing import Any, IO, Optional
import zipfile

from absl import app
from absl import flags
from absl import logging

import requests


DEFAULT_CACHE_DIR = pathlib.Path.home() / ".download_googletest_py_cache"

FLAG_CACHE_DIR = flags.DEFINE_string(
  name="cache_dir",
  default=str(DEFAULT_CACHE_DIR),
  help=f"""The directory into which downloaded files will be cached.
    Specify the empty string to use the default directory."""
)

FLAG_CACHE_ENABLED = flags.DEFINE_boolean(
  name="cache",
  default=True,
  help=f"""Whether to save downloaded files into the local cache."""
)


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
    return OpenedFilePathPair(path=self.synthetic_path, f=self.f)

  def rollback(self) -> None:
    self.f.close()

  def close(self) -> None:
    self.f.close()
    

class PersistentCacheFile(CacheFile):

  def __init__(self, git_commit: str, dest_file: pathlib.Path) -> None:
    self.git_commit = git_commit
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

  def __init__(self, git_commit: str) -> None:
    self.git_commit = git_commit

  def save(self, file_path: pathlib.Path) -> None:
    data = { "git_commit": str(self.git_commit) }
    with file_path.open("wt", encoding="utf8") as f:
      json.dump(data, f, ensure_ascii=False, indent=2, sort_keys=True)

  def __eq__(self, other: Any) -> bool:
    try:
      other_git_commit = other.git_commit
    except AttributeError:
      return False
    else:
      return other_git_commit == self.git_commit

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
    git_commit = decoded_json.get("git_commit")
    if git_commit is None:
      raise cls.StampDecodeError("git_commit key/value pair is missing")
    if not isinstance(git_commit, str):
      raise cls.StampDecodeError(f"git_commit should be a string, but got {type(git_commit)}")

    return cls(git_commit=git_commit)

  class StampDecodeError(Exception):
    pass


class GoogletestDownloader:

  def __init__(
    self,
    git_commit: str,
    dest_dir: pathlib.Path,
    cache_dir: Optional[pathlib.Path],
  ) -> None:
    self.git_commit = git_commit
    self.dest_dir = dest_dir
    self.cache_dir = cache_dir
    self.stamp_file = dest_dir / "stamp.json"

  def run(self) -> None:
    if self.is_stamp_file_valid():
      logging.info("Stamp file %s is valid; nothing to do", self.stamp_file)
      return

    if self.dest_dir.exists():
      logging.info("Deleting %s", self.dest_dir)
      shutil.rmtree(self.dest_dir)

    with self.download() as googletest_zip_file_path_pair:
      self.extract(googletest_zip_file_path_pair)

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

    expected_stamp = Stamp(git_commit=self.git_commit)
    if stamp != expected_stamp:
      logging.info("Validating stamp file %s failed: stamp does not equal the "
        "expected stamp", self.stamp_file)
      return False

    # Stamp file is valid.
    return True

  def write_stamp_file(self) -> bool:
    logging.info("Writing stamp to %s", self.stamp_file)
    stamp = Stamp(git_commit=self.git_commit)
    stamp.save(self.stamp_file)

  @contextlib.contextmanager
  def download(self) -> Generator[OpenedFilePathPair, None, None]:
    cache_dir = self.cache_dir
    download_dest_file_name = f"googletest_{self.git_commit}.zip"
    if not cache_dir:
      download_cache_file = TransientCacheFile(name_hint=download_dest_file_name)
    else:
      download_dest_file = cache_dir / download_dest_file_name

      # Use the previously-downloaded file, if it exists
      if download_dest_file.exists():
        logging.info("Using previously-downloaded file %s", download_dest_file)
        with download_dest_file.open("rb") as f:
          yield OpenedFilePathPair(download_dest_file, f)
          return

      download_cache_file = PersistentCacheFile(
        git_commit=self.git_commit,
        dest_file=download_dest_file,
      )

    with download_cache_file as download_cache_file_file_path_pair:
      download_file_path = download_cache_file_file_path_pair.path
      download_file = download_cache_file_file_path_pair.f
      url = f"https://github.com/google/googletest/archive/{self.git_commit}.zip"
      logging.info("Downloading %s to %s", url, download_file_path)

      session = requests.Session()
      response = session.get(url, stream=True)
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

      logging.info("Downloaded %s bytes from %s", f"{downloaded_bytes_count:,}", url)

      committed_file_path_pair = download_cache_file.commit()
      with committed_file_path_pair.f:
        yield committed_file_path_pair

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


def main(argv: Sequence[str]) -> None:
  if len(argv) < 2:
    raise app.UsageError("<git_commit> must be specified")
  if len(argv) < 3:
    raise app.UsageError("<dest_dir> must be specified")
  if len(argv) > 3:
    raise app.UsageError(f"unexpected argument: {argv[3]}")

  git_commit = argv[1]
  dest_dir = pathlib.Path(argv[2])
  cache_dir = pathlib.Path(FLAG_CACHE_DIR.value) \
    if FLAG_CACHE_ENABLED.value else None

  downloader = GoogletestDownloader(
    git_commit=git_commit,
    dest_dir=dest_dir,
    cache_dir=cache_dir,
  )
  downloader.run()


if __name__ == "__main__":
  app.run(main)
