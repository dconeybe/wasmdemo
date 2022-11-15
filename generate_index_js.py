import base64
import pathlib

from absl import app


def main(argv):
  if len(argv) < 4:
    raise app.UsageError("missing arguments")
  if len(argv) > 4:
    raise app.UsageError(f"unexpected argument: {argv[4]}")

  dest_file = pathlib.Path(argv[1])
  src_file = pathlib.Path(argv[2])
  wasm_file = pathlib.Path(argv[3])

  wasm = wasm_file.read_bytes()
  wasm_base64 = base64.b64encode(wasm).decode("US-ASCII")

  src = src_file.read_text("utf8")
  src_modified = src.replace("REPLACE_WITH_BASE64", wasm_base64)

  dest_file.write_text(src_modified, "utf8")


if __name__ == "__main__":
  app.run(main)
