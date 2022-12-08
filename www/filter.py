import argparse
import datetime
import base64
import pathlib

def main():
  arg_parser = argparse.ArgumentParser()
  arg_parser.add_argument("src_file")
  arg_parser.add_argument("dest_file")
  arg_parser.add_argument("wasm_file")
  arg_parser.add_argument("--mjs-export", action="append", default=[])
  parsed_args = arg_parser.parse_args()

  src_file = pathlib.Path(parsed_args.src_file)
  dest_file = pathlib.Path(parsed_args.dest_file)
  wasm_file = pathlib.Path(parsed_args.wasm_file)
  mjs_exports = tuple(parsed_args.mjs_export)

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
  with dest_file.open("wt", encoding="utf8") as f:
    f.write(src_modified)

    if len(mjs_exports) > 0:
      f.write("\n")
      f.write("export {")
      f.write(", ".join(mjs_exports))
      f.write("}\n")


if __name__ == "__main__":
  main()
