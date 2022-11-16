"""
Syntax: %s <git_commit> <dest_dir>
"""

import abc
from collections.abc import Generator, Sequence
import contextlib
import io
import os
import pathlib
import tempfile
from typing import Optional

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


class CacheFile(abc.ABC):

  @abc.abstractmethod
  def create(self) -> tuple[Optional[pathlib.Path], io.RawIOBase]:
    raise NotImplementedError()

  @abc.abstractmethod
  def commit(self) -> io.RawIOBase:
    raise NotImplementedError()

  @abc.abstractmethod
  def rollback(self) -> None:
    raise NotImplementedError()

  @abc.abstractmethod
  def close(self) -> None:
    raise NotImplementedError()

  def __enter__(self) -> tuple[Optional[pathlib.Path], io.RawIOBase]:
    return self.create()

  def __exit__(self, exc_type, exc_val, exc_tb):
    self.close()


class TransientCacheFile(CacheFile):

  def create(self) -> tuple[Optional[pathlib.Path], io.RawIOBase]:
    self.f = tempfile.TemporaryFile()
    return (None, self.f)

  def commit(self) -> io.RawIOBase:
    return self.f

  def rollback(self) -> None:
    self.f.close()

  def close(self) -> None:
    self.f.close()
    

class PersistentCacheFile(CacheFile):

  def __init__(self, git_commit: str, dest_file: pathlib.Path) -> None:
    self.git_commit = git_commit
    self.dest_file = dest_file
    self.f = None

  def create(self) -> tuple[Optional[pathlib.Path], io.RawIOBase]:
    if not self.dest_file.parent.exists():
      self.dest_file.parent.mkdir(parents=True, exist_ok=True)
    (temp_file_handle, temp_file_path) = tempfile.mkstemp(
      f"_{self.dest_file.name}", dir=self.dest_file.parent)
    self.f = os.fdopen(temp_file_handle, 'wb')
    self.path = pathlib.Path(temp_file_path)
    return (self.path, self.f)

  def commit(self) -> io.RawIOBase:
    try:
      self.f.close()
      self.f = None
      logging.info("Renaming %s to %s", self.path, self.dest_file.name)
      self.path.rename(self.dest_file)
    except:
      logging.info("Deleting %s", self.path)
      self.path.unlink(missing_ok=True)
      raise

    return self.dest_file.open("rb")

  def rollback(self) -> None:
    try:
      self.f.close()
    finally:
      logging.info("Deleting %s", self.name)
      self.path.unlink(missing_ok=True)

  def close(self) -> None:
    if self.f:
      self.rollback()


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

  def run(self) -> None:
    if self.is_stamp_file_valid():
      return
    with self.download() as googletest_zip_file:
      pass#self.extract(googletest_zip_file)

  def is_stamp_file_valid(self) -> bool:
    return False

  @contextlib.contextmanager
  def download(self) -> Generator[io.RawIOBase, None, None]:
    cache_dir = self.cache_dir
    if not cache_dir:
      download_cache_file = TransientCacheFile()
    else:
      download_dest_file = cache_dir / f"googletest_{self.git_commit}.zip"

      # Use the previously-downloaded file, if it exists
      if download_dest_file.exists():
        logging.info("Using previously-downloaded file %s", download_dest_file)
        with download_dest_file.open("rb") as f:
          yield f
          return

      download_cache_file = PersistentCacheFile(
        git_commit=self.git_commit,
        dest_file=download_dest_file,
      )

    with download_cache_file as (download_file_path, download_file):
      url = f"https://github.com/google/googletest/archive/{self.git_commit}.zip"
      if (download_file_path):
        logging.info("Downloading %s to %s", url, download_file_path)
      else:
        logging.info("Downloading %s", url)

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

      with download_cache_file.commit() as committed_file:
        yield committed_file


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
