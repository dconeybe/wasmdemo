#include "md5_testing.h"

namespace wasmdemo::md5::testing {

std::uint8_t UnsignedCharFromHexDigit(char digit) {
  switch (digit) {
    case '0':
      return 0x0;
    case '1':
      return 0x1;
    case '2':
      return 0x2;
    case '3':
      return 0x3;
    case '4':
      return 0x4;
    case '5':
      return 0x5;
    case '6':
      return 0x6;
    case '7':
      return 0x7;
    case '8':
      return 0x8;
    case '9':
      return 0x9;
    case 'a':
      return 0xA;
    case 'b':
      return 0xB;
    case 'c':
      return 0xC;
    case 'd':
      return 0xD;
    case 'e':
      return 0xE;
    case 'f':
      return 0xF;
  }
  abort(); // unrecognized hex digit
}

std::array<std::uint8_t, 16> Uint8ArrayFromHexDigest(const std::string& s) {
  if (s.length() != 32) {
    abort();
  }
  std::array<std::uint8_t, 16> result;
  for (int i = 0; i < 16; ++i) {
    std::uint8_t c1 = UnsignedCharFromHexDigit(s[i * 2]);
    std::uint8_t c2 = UnsignedCharFromHexDigit(s[(i * 2) + 1]);
    result[i] = (c1 << 4) | c2;
  }
  return result;
}

}  // namespace wasmdemo::md5::testing
