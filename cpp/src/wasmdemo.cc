#include <cstdlib>

#include <algorithm>
#include <string>

#include "wasmdemo/macros.h"

WASM_IMPORT("base", "log")
void log(int num);

WASM_EXPORT("echo")
void echo(int num) {
  log(num);
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
