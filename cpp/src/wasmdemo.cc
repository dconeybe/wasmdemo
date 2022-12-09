#include <cstdlib>

#include <algorithm>
#include <string>

#include "wasmdemo/macros.h"
#include "wasmdemo/wasmdemo.h"

WASM_IMPORT("base", "log")
void log(const char* s, int32_t len);

WASM_EXPORT("echo")
void echo(const char* s, int32_t len) {
  log(s, len);
}

WASM_EXPORT("echo_signed_unsigned")
void echo_signed_unsigned(int32_t sint, uint32_t usint) {
  const auto s = std::string("") + "signed=" + std::to_string(sint)
      + " unsigned=" + std::to_string(usint);
  log(s.c_str(), static_cast<int32_t>(s.length()));
}

WASM_EXPORT("add")
int add(int num1, int num2) {
  return num1 + num2;
}

WASM_EXPORT("reverse_string")
void reverse_string(char* data, int size) {
  if (size < 0) {
    std::abort();
  }
  std::string s(data, static_cast<std::string::size_type>(size));
  std::reverse(s.begin(), s.end());
  for (int i=0; i<size; i++) {
    data[i] = s[static_cast<std::string::size_type>(i)];
  }
}

WASM_EXPORT("malloc")
void* my_wasm_malloc(int size) {
  if (size < 0) {
    abort();
  }
  return malloc(static_cast<size_t>(size));
}

WASM_EXPORT("free")
void my_wasm_free(void* ptr) {
  free(ptr);
}
