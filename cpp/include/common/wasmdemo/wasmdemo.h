#ifndef WASMDEMO_CPP_INCLUDE_COMMON_WASMDEMO_WASMDEMO_H_
#define WASMDEMO_CPP_INCLUDE_COMMON_WASMDEMO_WASMDEMO_H_

#include "wasmdemo/macros.h"

WASM_IMPORT("base", "log")
void log(const char* s, int32_t len);

WASM_EXPORT("echo")
void echo(const char* s, int32_t len);

WASM_EXPORT("add")
int add(int num1, int num2);

WASM_EXPORT("reverse_string")
void reverse_string(char* data, int size);

#endif  // WASMDEMO_CPP_INCLUDE_COMMON_WASMDEMO_WASMDEMO_H_
