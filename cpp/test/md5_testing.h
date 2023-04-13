#ifndef WASMDEMO_CPP_TEST_MD5_TESTING_H_
#define WASMDEMO_CPP_TEST_MD5_TESTING_H_

#include <array>
#include <cstdint>
#include <string>

namespace wasmdemo::md5::testing {

// Gets the unsigned char corresponding to the given hex digit.
// The digit must be one of '0', '1', ... , '9', 'a', 'b', ... , 'f'.
// The lower 4 bits of the returned value will be set and the rest will be 0.
std::uint8_t UnsignedCharFromHexDigit(char digit);

// Calculates the 16-byte uint8_t array represented by the given hex
// string. The given string must be exactly 32 characters and each character
// must be one that is accepted by the UnsignedCharFromHexDigit() function.
// e.g. "fc3ff98e8c6a0d3087d515c0473f8677".
// The `md5sum` command from GNU coreutils can be used to generate a string to
// specify to this function.
// e.g.
// $ printf 'hello world!' | md5sum -
// fc3ff98e8c6a0d3087d515c0473f8677 -
std::array<std::uint8_t, 16> Uint8ArrayFromHexDigest(const std::string &);

}  // namespace wasmdemo::md5::testing

#endif  // WASMDEMO_CPP_TEST_MD5_TESTING_H_
