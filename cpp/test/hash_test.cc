#include <string>

#include "wasmdemo/hash.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

using testing::ElementsAre;
using testing::Not;

// Returns the hex character for the lower 8 bits of the given value.
// e.g. '0', '8', 'B', and 'F'.
char hex_char_from_lower_4bits(unsigned char c) {
  switch (c & 0xF) {
    case 0:
      return '0';
    case 1:
      return '1';
    case 2:
      return '2';
    case 3:
      return '3';
    case 4:
      return '4';
    case 5:
      return '5';
    case 6:
      return '6';
    case 7:
      return '7';
    case 8:
      return '8';
    case 9:
      return '9';
    case 10:
      return 'A';
    case 11:
      return 'B';
    case 12:
      return 'C';
    case 13:
      return 'D';
    case 14:
      return 'E';
    case 15:
      return 'F';
    default:
      return 'Z'; // should never get here
  }
}

// Converts the return value from hash() to a hex string.
// e.g. "D41D8CD98F00B204E9800998ECF8427E"
std::string hex_digest_from_hash_result(unsigned char* hash_result) {
  std::string hex_digest;
  for (int i=0; i<16; i++) {
    hex_digest += hex_char_from_lower_4bits(hash_result[i] >> 4);
    hex_digest += hex_char_from_lower_4bits(hash_result[i]);
  }
  return hex_digest;
}

TEST(wasmdemo, hash_ShouldHaveExpectedEmptyStringHash) {
  unsigned char* hash_result = hash("", 0);
  const std::string hash_result_hex = hex_digest_from_hash_result(hash_result);
  EXPECT_EQ(hash_result_hex, "D41D8CD98F00B204E9800998ECF8427E");
}

TEST(wasmdemo, hash_ShouldHaveExpectedAbcStringHash) {
  unsigned char* hash_result = hash("abc", 3);
  const std::string hash_result_hex = hex_digest_from_hash_result(hash_result);
  EXPECT_EQ(hash_result_hex, "900150983CD24FB0D6963F7D28E17F72");
}

TEST(wasmdemo, hash_ShouldHaveExpectedLongStringHash) {
  char data[1024];
  for (int i=0; i<static_cast<int>(sizeof(data)); i++) {
    data[i] = static_cast<char>(i);
  }
  unsigned char* hash_result = hash(data, sizeof(data));
  const std::string hash_result_hex = hex_digest_from_hash_result(hash_result);
  EXPECT_EQ(hash_result_hex, "B2EA9F7FCEA831A4A63B213F41A8855B");
}

} // namespace
