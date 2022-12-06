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
  char* start = data;
  char* end = data + size - 1;
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
