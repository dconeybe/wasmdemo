#ifndef WASMDEMO_CPP_INCLUDE_COMMON_WASMDEMO_MD5_H_
#define WASMDEMO_CPP_INCLUDE_COMMON_WASMDEMO_MD5_H_

#include <cstdint>

#include "wasmdemo/macros.h"

namespace wasmdemo::md5 {

constexpr int kDigestLength = 16;

WASM_EXPORT("CalculateMd5Digest")
void CalculateMd5Digest(const uint8_t* data, int32_t len, uint8_t dest[kDigestLength]);

}  // namespace wasmdemo::md5

#endif  // WASMDEMO_CPP_INCLUDE_COMMON_WASMDEMO_MD5_H_