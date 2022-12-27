#ifndef WASMDEMO_CPP_INCLUDE_COMMON_WASMDEMO_BLOOM_H_
#define WASMDEMO_CPP_INCLUDE_COMMON_WASMDEMO_BLOOM_H_

#include "wasmdemo/macros.h"

class BloomFilter {
 public:
  BloomFilter(const uint8_t* bitmap, uint32_t bitmapLength, uint32_t padding, uint32_t hashCount);

  ~BloomFilter();

  bool mightContain(char* value, uint32_t valueLength);

 private:
  uint64_t _size;
  uint8_t* _bitmap;
  uint32_t _hashCount;
  uint64_t getBitIndex(uint64_t num1, uint64_t num2, uint64_t index);

  bool isBitSet(uint64_t n);
};


WASM_EXPORT("newBloomFilter")
BloomFilter* newBloomFilter(const int8_t* bitmap, int32_t bitmapLength, int32_t padding, int32_t hashCount);

WASM_EXPORT("deleteBloomFilter")
void deleteBloomFilter(BloomFilter* instance);

WASM_EXPORT("mightContain")
bool mightContain(BloomFilter* filter, char* value, int32_t valueLength);

#endif  // WASMDEMO_CPP_INCLUDE_COMMON_WASMDEMO_BLOOM_H_
