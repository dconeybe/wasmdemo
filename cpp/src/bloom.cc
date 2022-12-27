#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "wasmdemo/hash.h"
#include "wasmdemo/macros.h"
#include "wasmdemo/bloom.h"

/// bloom filter code starts here

BloomFilter::BloomFilter(const uint8_t* bitmap, uint32_t bitmapLength, uint32_t padding, uint32_t hashCount)
    : _size(bitmapLength * 8 - padding), _hashCount(hashCount) {
  _bitmap = static_cast<uint8_t*>(malloc(bitmapLength));
  memcpy(_bitmap, bitmap, bitmapLength);
}

BloomFilter::~BloomFilter(){
  free(_bitmap);
}

bool BloomFilter::mightContain(char* value, uint32_t valueLength) {
  if (_size == 0 || valueLength == 0) {
    return false;
  }

  uint8_t outputHash[16];
  MD5_CTX hashContext;
  MD5_Init(&hashContext);
  MD5_Update(&hashContext, value, valueLength);
  MD5_Final(outputHash, &hashContext);

  // Interpret the size 16 char array as a size 2 int64 array
  auto* castOutputHash = reinterpret_cast<uint64_t*>(outputHash);

  uint64_t hash1 = castOutputHash[0];
  uint64_t hash2 = castOutputHash[1];

  for (uint32_t i = 0; i < _hashCount; i++) {
    uint64_t index = getBitIndex(hash1, hash2, i);
    if (!isBitSet(index)) {
      return false;
    }
  }
  return true;
}

uint64_t BloomFilter::getBitIndex(uint64_t num1, uint64_t num2, uint64_t index) {
  // Calculate hashed value h(i) = h1 + (i * h2).
  uint64_t hashValue = num1 + (num2 * index);
  return hashValue % _size;
}

bool BloomFilter::isBitSet(uint64_t n) {
  return (_bitmap[n / 8] & (0x01 << (n % 8))) != 0;
}

/// bloom filter code ends here

WASM_EXPORT("newBloomFilter")
BloomFilter* newBloomFilter(const int8_t* bitmap, int32_t bitmapLength, int32_t padding, const int32_t hashCount) {
  return new BloomFilter(reinterpret_cast<const uint8_t*>(bitmap),
                         static_cast<uint32_t>(bitmapLength),
                         static_cast<uint32_t>(padding),
                         static_cast<uint32_t>(hashCount));
}

WASM_EXPORT("deleteBloomFilter")
void deleteBloomFilter(BloomFilter* instance) {
  delete instance;
}

WASM_EXPORT("mightContain")
bool mightContain(BloomFilter* filter, char* value, int32_t valueLength) {
  return filter->mightContain(value, static_cast<uint32_t>(valueLength));
}
