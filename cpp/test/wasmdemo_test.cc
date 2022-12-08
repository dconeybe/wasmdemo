#include <string>

#include "wasmdemo/wasmdemo.h"

#include "wasmdemo_imports_impl.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

using testing::ElementsAre;

TEST(wasmdemo, reverse_string_ShouldHandleEmptyString) {
  std::string s;
  reverse_string(s.data(), static_cast<int>(s.length()));
  ASSERT_EQ(s, "");
}

TEST(wasmdemo, reverse_string_ShouldNotChangeStringLength1) {
  std::string s("Z");
  reverse_string(s.data(), static_cast<int>(s.length()));
  ASSERT_EQ(s, "Z");
}

TEST(wasmdemo, reverse_string_ShouldSwapCharsInStringLength2) {
  std::string s("AZ");
  reverse_string(s.data(), static_cast<int>(s.length()));
  ASSERT_EQ(s, "ZA");
}

TEST(wasmdemo, reverse_string_ShouldReverseStringLength3) {
  std::string s("ABC");
  reverse_string(s.data(), static_cast<int>(s.length()));
  ASSERT_EQ(s, "CBA");
}

TEST(wasmdemo, reverse_string_ShouldReverseStringLength4) {
  std::string s("ABCD");
  reverse_string(s.data(), static_cast<int>(s.length()));
  ASSERT_EQ(s, "DCBA");
}

TEST(wasmdemo, reverse_string_ShouldReverseLongStringWithEvenLength) {
  std::string s("The quick brown fox jumped over the lazy dog");
  reverse_string(s.data(), static_cast<int>(s.length()));
  ASSERT_EQ(s, "god yzal eht revo depmuj xof nworb kciuq ehT");
}

TEST(wasmdemo, reverse_string_ShouldReverseLongStringWithOddLength) {
  std::string s("The quick brown fox jumped over the lazy dog!");
  reverse_string(s.data(), static_cast<int>(s.length()));
  ASSERT_EQ(s, "!god yzal eht revo depmuj xof nworb kciuq ehT");
}

TEST(wasmdemo, log_ShouldCallTheImportedLogFunction) {
  LogCallCapturer log_call_capturer;
  log(42);
  log(24);
  log(999);
  EXPECT_THAT(log_call_capturer.calls(), ElementsAre(42, 24, 999));
}

TEST(wasmdemo, add) {
  EXPECT_EQ(add(0, 0), 0);
  EXPECT_EQ(add(0, 1), 1);
  EXPECT_EQ(add(0, -1), -1);
  EXPECT_EQ(add(1, 1), 2);
  EXPECT_EQ(add(-1, -1), -2);
  EXPECT_EQ(add(123, 456), 579);
  EXPECT_EQ(add(-123, 456), 333);
}

} // namespace
