#ifndef _WASMDEMO_WASMDEMO_H_
#define _WASMDEMO_WASMDEMO_H_

#include "wasmdemo/macros.h"

WASM_IMPORT("base", "log")
void log(int num);

WASM_EXPORT("echo")
void echo(int num);

WASM_EXPORT("add")
int add(int num1, int num2);

WASM_EXPORT("reverse_string")
void reverse_string(char* data, int size);

#endif // _WASMDEMO_WASMDEMO_H_
