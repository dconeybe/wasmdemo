#include <cstdlib>
#include <iostream>

#include "wasmdemo/wasmdemo.h"

#include "wasmdemo_imports_impl.h"

namespace {

std::vector<int>* gLogCallDest = nullptr;

} // namespace

WASM_IMPORT("base", "log")
void log(int num) {
  if (gLogCallDest) {
    gLogCallDest->push_back(num);
  }
}

LogCallCapturer::LogCallCapturer() {
  if (gLogCallDest) {
    std::cerr << "ASSERTION FAILURE in " << __FILE__ << ":" << __LINE__
      << ": gLogCallDest is already set" << std::endl;
    abort();
  }
  gLogCallDest = &calls_;
}

LogCallCapturer::~LogCallCapturer() {
  if (gLogCallDest != &calls_) {
    std::cerr << "ASSERTION FAILURE in " << __FILE__ << ":" << __LINE__
      << ": gLogCallDest is set to a different vector" << std::endl;
    abort();
  }
  gLogCallDest = nullptr;
}
