"""
Syntax: %s <git_commit> <dest_dir>
"""

from collections.abc import Sequence
import pathlib
from typing import Optional

from absl import app
from absl import flags
from absl import logging


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
    pass


def main(argv: Sequence[str]) -> None:
  if len(argv) < 3:
    raise app.UsageError("missing arguments")
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
