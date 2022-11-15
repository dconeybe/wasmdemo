#define WASM_EXPORT(zz_export_name_zz) \
  __attribute__((export_name(zz_export_name_zz))) \
  extern "C"

#define WASM_IMPORT(zz_module_name_zz, zz_field_name_zz) \
  __attribute__((import_module(zz_module_name_zz))) \
  __attribute__((import_name(zz_field_name_zz))) \
  extern

WASM_IMPORT("base", "log")
void log(int num);

WASM_EXPORT("add")
int add(int num1, int num2) {
  log(num1);
  return num1 + num2;
}

WASM_EXPORT("reverse_string")
void reverse_string(char* data, int size) {
  char* start = data;
  char* end = data + size;
  while (start != end) {
    char tmp = *start;
    *start = *end;
    *end = tmp;
    start++;
    if (start == end) {
      break;
    }
    end--;
  }
}
