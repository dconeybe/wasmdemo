#include <iostream>

#include "wasmdemo/wasmdemo.h"

WASM_IMPORT("base", "log")
void log(int num) {
  std::cout << num << " logged from WebAssembly!" << std::endl;
}
