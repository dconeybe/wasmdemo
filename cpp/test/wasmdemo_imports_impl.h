#ifndef _WASMDEMO_IMPORTS_IMPL_H_
#define _WASMDEMO_IMPORTS_IMPL_H_

#include <vector>

class LogCallCapturer {
 public:
  LogCallCapturer();
  ~LogCallCapturer();

  [[nodiscard]] std::vector<int> calls() const {
    return calls_;
  }

 private:
  std::vector<int> calls_;
};

#endif // _WASMDEMO_IMPORTS_IMPL_H_
