#include <string>

#include "wasmdemo/wasmdemo.h"
#include "wasmdemo/hash.h"
#include "wasmdemo/bloom.h"

#include "wasmdemo_imports_impl.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

using testing::ElementsAre;
using testing::Not;

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
  log("", 0);
  log("a", 1);
  log("hello", 5);
  log("abcdefghijklmnop", 6);
  EXPECT_THAT(log_call_capturer.calls(), ElementsAre(
      std::string(""),
      std::string("a"),
      std::string("hello"),
      std::string("abcdef")));
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

TEST(wasmdemo, hash_ShouldHaveExpectedEmptyStringHash) {
  std::string empty("");
  auto* obtainedHashPtr = reinterpret_cast<uint64_t*>(hash(empty.data(), 0));
  std::vector<uint64_t> obtainedHash(obtainedHashPtr, obtainedHashPtr + 2);
  ASSERT_THAT(obtainedHash, ElementsAre(0xd41d8cd98f00b204, 0xe9800998ecf8427e));
}

TEST(wasmdemo, hash_ShouldNotHaveZeroEmptyStringHash) {
  std::string empty("");
  auto* obtainedHashPtr = reinterpret_cast<uint64_t*>(hash(empty.data(), 0));
  std::vector<uint64_t> obtainedHash(obtainedHashPtr, obtainedHashPtr + 2);
  ASSERT_THAT(obtainedHash, Not(ElementsAre(0, 0)));
}

TEST(wasmdemo, hash_ShouldHaveExpectedAbcStringHash) {
  std::string abc("abc");
  auto* obtainedHashPtr = reinterpret_cast<uint64_t*>(hash(abc.data(), abc.length()));
  std::vector<uint64_t> obtainedHash(obtainedHashPtr, obtainedHashPtr + 2);
  ASSERT_THAT(obtainedHash, ElementsAre(0x900150983cd24fb0, 0xd6963f7d28e17f72));
}

TEST(wasmdemo, hash_ShouldHaveExpectedHashAfterMultipleHashes) {
  std::string empty("");
  auto* obtainedHashPtr1 = reinterpret_cast<uint64_t*>(hash(empty.data(), 0));
  std::vector<uint64_t> obtainedHash1(obtainedHashPtr1, obtainedHashPtr1 + 2);
  EXPECT_THAT(obtainedHash1, ElementsAre(0xd41d8cd98f00b204, 0xe9800998ecf8427e));

  std::string abc("abc");
  auto* obtainedHashPtr2 = reinterpret_cast<uint64_t*>(hash(abc.data(), abc.length()));
  std::vector<uint64_t> obtainedHash2(obtainedHashPtr2, obtainedHashPtr2 + 2);
  ASSERT_THAT(obtainedHash2, ElementsAre(0x900150983cd24fb0, 0xd6963f7d28e17f72));
}

} // namespace
