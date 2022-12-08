#include <string>

#include "wasmdemo/wasmdemo.h"

#include "gtest/gtest.h"

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
