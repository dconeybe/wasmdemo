import argparse
import dataclasses
import logging
import pathlib
import sys


def main():
  logging.basicConfig(
    level=logging.INFO,
    format="%(filename)s %(message)s",
  )

  arg_parser = argparse.ArgumentParser()
  arg_parser.add_argument("files", nargs="+")
  arg_parser.add_argument("-i", "--in-place", action="store_true", default=False)
  parsed_args = arg_parser.parse_args()

  paths = tuple(PathStrPair(pathlib.Path(p), p) for p in parsed_args.files)
  in_place_edit = parsed_args.in_place
  del arg_parser, parsed_args

  for path_str_pair in paths:
    guard = calculate_header_guard(path_str_pair.path)
    if in_place_edit:
      add_header_guard(path_str_pair, guard)
    else:
      logging.info("%s %s", path_str_pair.raw_string, guard)


@dataclasses.dataclass(frozen=True)
class PathStrPair:
  path: pathlib.Path
  raw_string: str


def calculate_header_guard(path: pathlib.Path) -> str:
  components = []

  while True:
    components.extend(chunk.upper() for chunk in reversed(path.resolve().name.split(".")))
    path = path.parent
    if (path / "wasm32.toolchain.cmake").exists():
      break

  return "WASMDEMO_" + "_".join(reversed(components)) + "_"


def add_header_guard(path_str_pair: PathStrPair, guard: str) -> None:
  logging.info("Adding header guard to %s: %s", path_str_pair.raw_string, guard)
  path = path_str_pair.path

  orig_bytes = path.read_bytes()

  with path.open("wb") as f:
    f.write(f"#ifndef {guard}\n".encode("utf8"))
    f.write(f"#define {guard}\n".encode("utf8"))
    f.write(orig_bytes)
    f.write(f"#endif  // {guard}\n".encode("utf8"))


if __name__ == "__main__":
  main()
