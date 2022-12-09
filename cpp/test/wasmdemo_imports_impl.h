#ifndef _WASMDEMO_IMPORTS_IMPL_H_
#define _WASMDEMO_IMPORTS_IMPL_H_

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

#endif // _WASMDEMO_IMPORTS_IMPL_H_
