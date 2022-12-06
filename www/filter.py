import datetime
import base64
import hashlib
import pathlib
import sys

def main():
  if len(sys.argv) < 4:
    print("missing arguments", file=sys.stderr)
    sys.exit(2)
  if len(sys.argv) > 4:
    print(f"unexpected argument: {sys.argv[4]}", file=sys.stderr)
    sys.exit(2)

  src_file = pathlib.Path(sys.argv[1])
  dest_file = pathlib.Path(sys.argv[2])
  wasm_file = pathlib.Path(sys.argv[3])

  wasm = wasm_file.read_bytes()
  wasm_base64 = base64.b64encode(wasm).decode("US-ASCII")

  wasm_num_bytes = len(wasm)
  wasm_base64_num_bytes = len(wasm_base64.encode('utf8'))

  date_text = datetime.datetime.now().strftime("%c")

  src = src_file.read_text("utf8")
  src_modified = (
    src
    .replace("REPLACE_WITH_BASE64", wasm_base64)
    .replace("REPLACE_WITH_DATE", date_text)
    .replace("REPLACE_WITH_SIZE_RAW", f"{wasm_num_bytes} bytes")
    .replace("REPLACE_WITH_SIZE_BASE64", f"{wasm_base64_num_bytes} bytes")
  )

  dest_file.parent.mkdir(parents=True, exist_ok=True)
  dest_file.write_text(src_modified, "utf8")


if __name__ == "__main__":
  main()
