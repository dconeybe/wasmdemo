#include <cstdlib>
#include <iostream>

#include "wasmdemo/wasmdemo.h"

#include "wasmdemo_imports_impl.h"

namespace {

std::vector<std::string>* gLogCallDest = nullptr;

} // namespace

WASM_IMPORT("base", "log")
void log(const char* s, int32_t len) {
  if (len < 0) {
    abort();
  }
  if (gLogCallDest) {
    gLogCallDest->push_back(std::string(s, static_cast<std::string::size_type>(len)));
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
