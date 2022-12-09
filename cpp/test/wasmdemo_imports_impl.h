#ifndef WASMDEMO_CPP_TEST_WASMDEMO_IMPORTS_IMPL_H_
#define WASMDEMO_CPP_TEST_WASMDEMO_IMPORTS_IMPL_H_

#include <string>
#include <vector>

class LogCallCapturer {
 public:
  LogCallCapturer();
  ~LogCallCapturer();

  [[nodiscard]] std::vector<std::string> calls() const {
    return calls_;
  }

 private:
  std::vector<std::string> calls_;
};

#endif  // WASMDEMO_CPP_TEST_WASMDEMO_IMPORTS_IMPL_H_
