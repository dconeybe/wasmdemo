#include <cstdint>
#include "wasmdemo/hash.h"
#include "wasmdemo/macros.h"

/// bloom filter code starts here
class BloomFilter {
 public:
  BloomFilter(const uint8_t* bitmap, uint32_t bitmapLength, uint64_t padding, uint64_t hashCount)
  : _size(bitmapLength * 8 - padding), _bitmap(bitmap), _hashCount(hashCount) {
  }

  void reInitialize(const uint8_t* bitmap, uint32_t bitmapLength, uint64_t padding, uint64_t hashCount) {
    _size = bitmapLength * 8 - padding,
    _bitmap = bitmap;
    _hashCount = hashCount;
  }

  bool mightContain(char* value, uint32_t valueLength) {
    if (_size == 0 || valueLength == 0) {
      return false;
    }

    uint8_t outputHash[16];
    MD5_CTX hashContext;
    MD5_Init(&hashContext);
    MD5_Update(&hashContext, value, valueLength);
    MD5_Final(outputHash, &hashContext);

    // I think this is right??
    uint64_t hash1 = outputHash[0];
    uint64_t hash2 = outputHash[1];

    for (uint64_t i = 0; i < _hashCount; i++) {
      uint64_t index = getBitIndex(hash1, hash2, i);
      if (!isBitSet(index)) {
        return false;
      }
    }
    return true;
  }

 private:
  uint64_t _size;
  const uint8_t* _bitmap;
  uint64_t _hashCount;
  uint64_t getBitIndex(uint64_t num1, uint64_t num2, uint64_t index) {
    // Calculate hashed value h(i) = h1 + (i * h2).
    uint64_t hashValue = num1 + (num2 * index);
    return hashValue % _size;
  }

  bool isBitSet(uint64_t n) {
    // To retrieve bit n, calculate: (bitmap[n / 8] & (0x01 << (n % 8))).
    return (_bitmap[n / 8] & (0x01 << (n % 8))) != 0;
  }
};
/// bloom filter code ends here

BloomFilter theFilter {nullptr, 0, 0, 0};

WASM_EXPORT("initBloom")
void initBloom(const uint8_t* bitmap, uint32_t bitmapLength, uint64_t padding, const uint64_t hashCount) {
  theFilter.reInitialize(bitmap, bitmapLength, padding, hashCount);
}

WASM_EXPORT("mightContain")
bool mightContain(char* value, uint32_t valueLength) {
  return theFilter.mightContain(value, valueLength);
}
