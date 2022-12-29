#include <string>

#include "wasmdemo/bloom.h"
#include "wasmdemo/base64.h"

#include "gtest/gtest.h"

namespace {

const std::string documentPrefix =
    "projects/project-1/databases/database-1/documents/coll/doc";

std::vector<int8_t> decodeBitmap(std::string_view base64String) {
  std::string decodedString = base64_decode(base64String);
  std::vector<int8_t> decodedBitmap(decodedString.begin(), decodedString.end());

  return decodedBitmap;
}

TEST(wasmdemo, bloom_ShouldPassSmallGoldenTest) {
  const int TEST_SIZE = 2;
  const int expectedResults[TEST_SIZE] {1, 0};

  // { "bits": { "bitmap": "RswZ", "padding": 1 }, "hashCount": 16 }
  const std::vector<int8_t> decodedBitmap = decodeBitmap("RswZ");
  BloomFilter* bloom_filter = newBloomFilter(
      decodedBitmap.data(),
      static_cast<int32_t>(decodedBitmap.size()),
      1,
      16);

  for (int i = 0; i < TEST_SIZE; i++) {
    const std::string ithDocument = documentPrefix + std::to_string(i);
    EXPECT_EQ(expectedResults[i], mightContain(
        bloom_filter,
        ithDocument.c_str(),
        static_cast<int32_t>(ithDocument.length())));
  }

  deleteBloomFilter(bloom_filter);
}

TEST(wasmdemo, bloom_ShouldPassLargerGoldenTest) {
  const int TEST_SIZE = 10000;
  // This is inlined because, I think, there's no way to do IO in WASM
  // (and because this can be a bit hacky as a hackweek project)
  const int expectedResults[TEST_SIZE] {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  const std::vector<int8_t> decodedBitmap = decodeBitmap(
      "xdQcZ8NOMjvdJWzA/WSuWC32Bj32rIjsfRD1uVBs51n2feOWmw1NVL12gkJ2/E6aWLxnfo07GBDPCkCdJhVmqmXzIqpAHbTx0WTc7r+6ybJnHiJYonv4Vvte3EBu/+Nd9RSp0liy/FxW45djgQlVqKtYjK0cy8yNP2QANUYhHtRRHg7n1RC/nsze5oLUsYOCkjMPFZQlGFyRdXXoG90B0IePx361tLXYIDUcSiQMWg/xC9TnSiQ1iG430859gCXGmN0nn+HQoZveGYeaE9GDFmqBQR3LhZKBhonRZSDlX/yo2hpmPtDaxYwnVnuEGF5hztXDV2FeTFGViGgqv2cAz8Amk7AfeNayF1kynJDYa1/Styng+tWiegr/gZixN0ZbSKxd4p/YF/rhRKTOsZS1V4qcwm52SyWtQD2c35wb+i5D0930JZO9iLrOGCY2b8gMhyk57mB9f5zEkz+DIHpIXlF+lVN1sUAJOusGqPKWf2wbSGIYxbRjuvo+YuO4be04E3MoyfdikZhmr+KtIvF3ZBWlyaxNXaYRtfwFctYPXtoFMifo6FbT+f8oBAckOXvqMup3u2H5fYwAD8aoOU4QeuaVUraTulyEq3tI4nnbIwmJnB1rF4dP8xzwj6IwGk2rz1s4ASl4QkZXUUjZiN8I9/iF0QYijV2bZ6fo4zSEUw51QKj5KiYTSSB5skRHFW9jLA4AJKUjgyDmlYTm5Ocookr9p0VBJ/p3yowTm7yXb5FDscka4jxhljguik9g1YBc4XdDp019nwdoSnJ/FH4R0Kl0EsxkGJ701fDs2Gl/8c4YLbf1u6agaOlbQpVm5BMkzoA8/LQNmwxDsLGBnQwUacWUowwY5wQCBxW/yy7hF/TnFzsysAHTFxioZd2GHbt3LWblB2KbmJqVNAssrk3284c+jRLRGi0kCr8KciUztIBnJfaMiWb+ifD5jNDDZig74eRXiBEIcAyO5sSD/06of5FFfOIVnYFpLedzVkYPHMPNXrI/0iG+8PSJz4QzGmrn6PQiw3Oj6BTiekY9i4T3glRJiRYVDxdagWR5O6EaQP+3ZSBuYumjZfpFF42L2JSsdPvDfdEu6S9wU9pM9PCpptnE+Vxsl3oXAOF6/Ud+pOy2h20u0kVYTNGWQmeMt6JKxAnCMXKg40HHVDyPQwRVnreUN5lmuot0vUJwdWvM1yQFU8zND4WUWbgP6Bnf58FpmOwFQ9o5odcEnHqKyn410e1GyT6BbPZtAPZ4gSUDi6zxXgpJU+LDCqU104yLazZwIJhO1qIDZCAdvrrfUTZ4FmEBu9tAepVbq/BX8K1ScN/qVqJmUnIZwv22ArJgBvVEc+WfIbZbbLTG/COxSYnrvfCpAlrquvC3d/VACdMs5JwILSaJoGaGYSYevMKhDyn/01KY+ROV6zadBMpC7gg+7iiPlJHE1u/6EM0xfuH56iI9u5Aahw3hBC6yCXqVxsaNC3lb1nahz/Q75wpXHxRmVDEkfV6BCZy1Nol2m2P0vCA1tMzGgkqolQTfc0/yMJ07WVdiymDarQvwUfm/UhoAHfsd3AKChk/Jykf89W3oMiVGC73lQ7ZHVMOPDLhEQe8HR/bvurCRD7OL2TxFFDI0q1cxg4r+Yp6fZCEswoUeK756Bw3yDm4VgAQP72LoO9P85MtnEoBKdxVbBEBEHp3jCg+rHRvxyGGwQuz4s+6n3efgh5/gvdi+RvTHFql3RshljAVOI26MlW/3MutTpL8V2IaLx7xyU2mMBsagCGAEHh882O3hhk6xWk7Zf/26IBovYX+AWUvd2yF1yDSf5gb9Kb4ieBUvSBb9EEb78XOHnla4dgFTVt45+m7pSc5AOxJUJdIFD7+gv7OrO+ISJkrroBuHr/QYucsHzvQhnzaj+kpaVFFmSxtAYOlNXxKv7pekB6lQ0R9hnGWogBGPd9CoHvSfFyxTflT1Wx6kaUIBZbDEHcd/iDzfLK71Y559MH8A6ICgdUAHjbh5FlghK+Rn3xb02KdntXas5VjzYULg4DrVZpVHbU4ZxmEo6Ek3g3AjSs8t8Jf12mhFfcmiAXTsXbQxBjhVoUzUoPspt6wxt6QZZ9Ws9k0vFAsHpR1FyaGc2LuT1Tu24boK1q7TzBVWdX3Klih8ljQzI4wi2jLdm/+ghj3c2FUXgr2zznNIKuzn7C/WNqsaKmBezTzuq84dubx9rHwRI38+hVAIRyV2iiuX8IcYvtuU6cSsEWi6x9BBJTS0ZIdScn0KIAvvVrd1FJeqTZsw73N9ZA+pKMJWDSleR08CNjRqDz3/t4tjiFGHJYBwEl3EterPSJ2UC24fklmCcvw6oUuNqxKcykoqqOm+47opJL0inBleF5eUwphkwc//XJnVBzygB1GmBhU7v4QldcLz2IqbvBTBDN1HEYeY2yFzCa4SEVxZkWoxPVYCSbpzBuXZJIjDibcHp1v4qANYfF90f1TtdpKOKrzOwq4tKi5YFgBAIZx8eE/VZJcAhYfEMj1ECLhE+zAnQDXUvZngdzWDpCwAKEU1Qv2mAeDpb74rCfvXY6b9QqggzX7HmwrKw0LnLTO9kdUXW5iNxIs5ZhYKxlzoh8WGCS4z3cdMYXK5kg3ezDzqTkOo6IscgTPa6uS48Nlupz3gN52PVfb5HmuqyRtP9532MAbCkKTuMI/rR6MNuDt3Jzehg3RUGKIgkuY6biNnBBGNjWNQdo1zhrwozUMS7+9OjqSYzm0uyDAtiLu4hCYV+mhM6A1iYDKipTNdjHMQlN1XLOAVF1ygsbgOW237+M4oKmgOiec3tIh5cQAkyQlA7kZuydnuG+pdmCCo8LmCyWsH8os4rixVYl0Dgda7//IQ16GJP+bRd3J4F3ouXMZQRK0edquEw8U1N7q/ULFynvBt6nk8Aac+oSSmXmmQLieUYe7VJ5m2Fdrqcf8oCBhvCQTs9U1Ovsuk2QDg19OCVXsroThlGC+atSSvsFQNhEH7+w9pZt8qeTsCT0NGON8+saVbaXyAZNB1VETGrPKKjC3bfzS8amlqs24X/vIuIKY+udIrvTpBjlnYMmXo1F/3TZUl+FxjcepqjQj+lpLfaHDCGgy65qJgW0A+kbXc12tpA6tzyjoXUrNVNjfftl5GkIOpsPqzyfDNQuf2FjgiBjiOnd2jirbNxtjwK/Sp6fnpaNbpMqvrYZ7gAIIESJolJl2nGABoaqCdW1pAI8PSebl1dJogGmKLAKrkjrumnULssmdx4QFBz7m+hFt5AgJuiYmYir4vQWAgQS0T6xdgzmSRNQBPBMXnK8jVioEaZ+eE0zA3df2+NQ3mCcKaVMKGmO80FXrE6f6z9K3w5oc9Cwis9TEOAFdzAY+IkYs+H8mEHQwVSHDQdfo0+RV8q51pEoB56OzKPedGn1LiTfzVVLFiRUsL892tK9euktDnnZ6y5sDLNp6s0smDOROqQuLy+R4VeyFM1O0ZuEjjEjtQDZ3IobzM9ONmnz8hH7qPU/HYJq7JgbndYbSfRpudqj5uACyys1DD5KRiUvLNWRp0ZzqCbHzknjHZbva6q85J/zWUX3BnTgW8XbDLgpXTojWEKksPOw2zuwcURqsYOW+Hp2ElYJjBOTDjEDhHC7Tvl36ZtLDdB4NwDjJmdxXOXJRy/6y7xeDhTiKaKyeFMywFUvVgs4FEoCnNuhGOpcFUe0lHuYM4tp6KgnyWkOWI7uqtS1uXmYTJATotOXamx4y9DkeXYDMNxl7QsRSEK355AVwg6+iMCEqhzk6C5sr6rVOgdeoEiH0JEh4LnR3SbfrV9XYF9B0/BmeHqkl9Qwms0NZOuxXiIEaalA8La+NIuUhkI3OoHeNebAkcoYDI/SfPZZ9EOGHBrWp9p/heqrEY4v4TPOvjkXpuXmgasvMuxzAdlNDsrE3g/V6ltJdeP+KAptIRUXg6dPMJ29Vr6PRhcoahP8MkeBjBCEUeCJYOwAlbp5E+ZPBePduch4iPU0kfbWZIOxNW4WDKvhOcHfPwTs1xfzyTP3sHI5btb6zwSpGcJ9PQbx7gLNk3pMe+BCoWXLBtHO0r2TT1P4YJSxSJ0iYODzjV43LRSD4TuQwpKqzscIdkpn8rmqrk4gYbFAcX/Rf+OfQApY2yEb290pEQNeTGm/dQauEqzgrlKIsVB6g9NxQAK5SUduP9qC9CppL2SEeJgKK54ka2oWDi0vMuvJvBpjaJ9AuHv6B/NoftbN0Qb/7tSnkFAXsnf3XmCSmLjiujbEDku+63eGE1kEQvEagEsSqSktVPmmHsam4rXf3VGTR5GLSqqMdWBzTXM4PUKy5u2DeAqb5sLYGPy3VtytuaTkXVAhJ6I2/AbC63Wv6iax+j8VeZOhUQagUqCH+GjmiDTIzeTHsJ4e0K8uNXZMt/bGVRUJLOnb4YWFPyLEnLYduFPnRekF65cfuwTbbHEUAgr9ZezgKkAygCjF9tIiYmdqzEp+6jicW9Y4TzJb6tBxsxKjqe1YAYhbEHwHhQlG0oQ8FdKUB2FWPu1uyL+Ttht1MovhEspDbYaTmKUKtjwJZN3oBvza8UUXbiLaNEQz/e+xzKJGI7cMSF2IXucOTZ0LfTZ+ESrUZArtH1HfNOz5vWvXGVYtPvqPXb3TVD1eZwVxRaP37sC3XROmQ6wj6oEBaPyGCdAHZpBjmJ73ew3Fj/z/xY1DCTk9Wg1OvOU5nIlKnAK7g436cMaOPSz6bB6rwXPVfK/0fQqW91hXp6LOW9/Je/qoY4Ur16LgSpQ0GRkAm22tFj9baZX53Y7BWo9GGWu+rGWG/X2CUq5wOc7KfSJjUNSET99xHqfDyBXrCb9pDONZfyc0n8PcvnSSEtX9bu9Ou5fOVo5Njt96lRNikGje2j6K8EHBWnvqKwjcy/6IlNQccjhdY2FcAsAz44Ecw+KDPHLpjNklqwlt6SBNe66RAd18Hd9o4JwaNx5MGlzhkGr1JJ/GjslqegS2Qd4GiGI1iT9b54H/hMHmgjooPaBMIkopwOLj39LJbr4R9z5aLHO6FMmPDeIPfc3heNswwacJO9Zg9OMOO+R+AA52IXGyD/Qbh1H3ROqaxN1V9nTB1I8Ewu4Hc/z4L6LYvP5B2zQLmRyRElqBkbgeM1FUiQKNnQsF8EFFVGBy6Lni0trh281nuz0HkM7b9IrXVjUmGvAngcfzxi/J02QFta6iFB0Lr8Mik2J9MtkqPvUG4T+FuSTmndUa9T0MGpB5JadlkgOhmTY8UUd6lx5+Yc0PbnUvp/zIBwZQvOC3JCho1rBQlINUsIXP9SDbQ26SYJVLlC7WOhoGjwdy4wSYCIZY4CR/oaTnc5VymCGOri0ZR1DqnFx9Z/koVXYcBfJNdddlJR1WBbhUOCcfU/HciQvzoER6xaijmzllVCFyxhnhWcMtBuViyJpGBVRYBj6kitjjBPLvoCKBoYrn0VwjCPUcA2JQlLl6+RPqng27fLdhfSRHcUv/zNxi7fZcQN7Zep0fdN+KVggj+tFNUdWL8aAQqzQyHmPBnNeFJdn+11Oc4V2JuoDVBEpyjFCkaMA6F9qsXX/RQkdBiT+AnWdssx2kTwcmrdbYbcgjeWpfgSpquT2StXswcby7i3F9ismuQMNUxIEIQYccpfOtTtL9f3XivYgfjnVmkp6bwIYfcuLdNzjDlc89p0giO3VLxufboihV3tythbCQ3te9WherywYcL9ZOHB9ECf2u96HXbBSkIi2Sqaj4ISmL0f5KFN07kBLygCtmqjzRr20lun0XtEMOaolvLjbH2i5FZzNtL+GcUhVRbGh5q810KLpBrbhX09pdLaOQ0QgKh2DIvMSeDamPEI75omNBNrHFDg5xg93vZFFFdzGodLTp1NLuiCR1prs8Ou3BEBWPxzXc2SvRnjPee0jVP1Mlf9Umc2hXxFoQ3QV253j+Zpxi5LUoWr8DZm5L627YyeK8+gmhokdHRZsqgUoaUcU4xikoyK1Kp3LpfYpwLJ3saiVeJng9c+gObXzcZOOIB/IO/ilKVGmsl7BfFWA+clg58hNJpq40FDwjN1zMyu3hVie9iIj4bp2jfLZDQSrI2xhikXBHTln/aVkuXr9U2V7ch6RcQXf50rOPFeFA/w6elgvIVGzSPmL3K93czNQkHlJwE0qMGRAcQ2O34kfGObhDVu9s2ZBVqCUyNC0eVcl2U/uxnWwhBqpJKcJD8dbhJv9JYKsOwWjfjBDhF6xz0j0gQPK3vbf9GQxcHaS3X3tK366JdHFMBw22fOjby7LZpRGevR1+jPWHzPKFUI0Pdrkl5qTlLpSmzwL6kKKE0m6sgDmVPPubfe87B1A5HG2MESSK8hrAhveg2t5vLCogWsAdILAGe8Py3W5pOB8ZvOrGDNQ1myxK4ZbfPdj6eTF7w7bXA/5NtDO+UidYPWQgcvDCaFqPEl5Nt+5FamO1G2pgAW2aiXuvbjMiDRWxUZ7YqOYsJidMWn47mcq78qqcA6n9E7hwcgKkaAbS51O2h8tFJz0kH0kcvag8Cr5BaHWMv5LIB/XrPTUv6FCYHvVibtYzIQPabksMIROweN5kHYkjoQKaqp4P8xvlfnAJdY76G9pQ8p+m7bxbhRhcwwOM8ph6GoKDeFLKZF+p0wQ6O6KTbAs1soHvhrShNuQJDT+A9/d0c6cV6ofnVWwtcSwAWh0iESvjKH+Qtb1SxB2/qvuDIHaYo+bsGDhBlfdOE7k76cdn6L8mcyCILI+ebdsDti+cg18dKjrotRjMRvGk/+8jGLtw/EPjMQYXHNSmzs3QKqNBQjWJDum+oPaxJ96hzo6KRQwt15YPR2fokGl+fP0cUYUoQ4FEYQLMvlHPfy6GToAq2trScwPuKkGEwOWDQwZ9FREchuRroZQD3uM4O2ou/KEzmhPkAqXXO53GpJ9c9bkxEcCXA4fmhcRTXRnGJYVbNHkyEoIexVpjSL9u/aMPabEnEFSm3ibEHzEQ3DgfakDa0xNv14cjdTAZCEDJa1FXBmGcs/H94RZH+QUuldI2r8/oXBlV8FRcdXk7YoiAI/ehZLtJAqs4M1Q5BMBqyiN0DmTY3t4W7+mn4Mc3SG7874KegRZvKXOlCJjKB14Z3SuzbsifH1UrGS68pNWpwmBZavTo3k8j5WAD3t5m0Fu0Z8Ajq3rsVJrj3bEFo7Qq8eMV9WvJgFQbL52r40WjslAKIdhkNgPYemK8/1z1V/VXQ5oZ3SDaT04RNlJsgHMw/WrUtY9xOAUk2bqdgxKXT0yo6/4ZgoX2AkSbUCcfWAAC+9hY8jF+trMNNil//9zttTFIbAQNG9My4IH6Di0BTQCDH2yeGZhkzEugvZGLQ9dRZZR11pKS2wiC4uys6IRxigsGGoWuxbeuLFg3395A1u1jLlpGAAKj/L6bWMeanwmwnHjdT47xYcVjJGVPJXQZhH17DS8zyaGpNWjk0CQCfq76vxlY0/mM4YvKBvbPVAgwEIN81F2kX0CJFa90LDG7ufhMsRDlMoobgXUdrq8aqfQ7ZcXKSjQqw6GagNZgjYkTL7COsU+W81Y5xER3DznCElvhmZx+/hhDiQsAxD4w75bg+aJd0nRSTkau50d9qgmY7/qPgYpWHItYt2N/Zm75EgfEBq/H5ojzkepHpLYuTZV/KwUOJgpxoluFNd/crK3Og5FnkqwKm8N1qjP7vkNrUbw3fpsYNtWO575mMggNZqfREfFd2s5g5iq4QdMfcseC2BBNGLzG8q8mWOpAGCJ6tjfj0CWoKE5QiES6baZvWEUY5DvDQkRI23JHxpurx4m1nfwkQ5gBJe658fIluOuq5+T6pWDoqHsJsXpPybxlCu+MAY+mdBG8m/XFzFxiXWbIAMbNthYtqKaHm27zuNCuPUsWdRAn8TjGru6hmBq7qn3UX2rZqvmsypTsmjRLOXrJL9n3lq92dskLHC/rIbO9IalmA25FJGn8twe962dAlXkk61FEUsGfcJbGfzBGFSmT5SiDS/jOWyRs6F1QJzDPl6RFEDKinIj4/jj0mC1r033BP3zPbUrRLiJlWJk3EutAPGShg3VEczKlX8fQ74CbzIcbXoqr9BITO2K+hl7vxIWjOTezXZVDHzRfWZ1hKMmAz7gR6znXHLCmh3Fb20kauWMbkCkOJyjL2rErUxHC020SAIskRd3SLpvGdo9SSQU84CwpELM+MDyWwvA1COGudRGQ0dm46kwoFfkzsJJh5W23VkrJKLOfmtnLkVlHo/Dg0qX7i7qqqxYu3rSj2KwFJDS4/zM5vxYaOMLLSMqpf4jnPqxb71hHerbhtovvIekWtHyBvrmG2USSvHHVOD7Rvm/QGOZFWq2iExubURVDkPO4YBmtT4U2Gug8+QLKCW1h2ryTtOqCjmchFfu0TTlQcGKxCqozNp8AhC77eDQQd30JrNS9Q+1oq2pAECBQ/iaiaTAPLQNfJiWExB2ZBG0qZOE1rRZ/6UR4A6sYB8zs4xciUjNsA5+kcBge2UJenrKxMPtE56JuIZQqcg7EXwRBItnWfWanDFHJZyKNk+m1MAChOoX9g7q28yKtyOPSQR131cATxAbIf9kfEVOdzg19xU6FC1fxZwLPjySTuCbwcdagLlLeTg9fu9LmB+012S5ofO6ozoLhQDXFeESRAP7kTZ7BNlKZItSylC6H3mbTtgCHfraT0l3gE9HuJSZlGiB2hhzhRBZYS23yKtujZR4G2wQoZhOmvJ325NrVzeicV8Xlq+QJpb5Uddv40EI11oXFRGa5GaoP/43o/cVg3ULVcyM84r0H0KUU+v64m4w+AyYdP6FoK/1EgLmL5Fj8w8pwcr/d0/apJB7DqQtaTdS3JaozNOWtBQoNwu9PID+FXrJhPzxTS/Sru/igMUSBlbyygBVkVvApguk5l277lOaSEsUBHgGHo7lARlpwZU5DcpBzxCi0hLrZr99VVfA4PbPlmUk1rJXWDy9t8moCBrOAvB6PlY/gfKaSauZ3MFaQMIqyjVPPrhuajzqzl3El1sFd45kORJABOfQ0UG4CsHkHMmle6hWrhO4BTLsDPkab28pM0yHMghZl0VC6SqBUWWnz6MdYR67PllCTDg5TJcUVplQBnlt/2ZeOmgrAszE7v3kT5zHg+yv3YZm+86CkJMOkNJPxknQtbHBJU6tRNOIhtMI9Ho5GybyzEASFkzeyAs4zSVO4o0U3kWDPi8r6YihOiu9P9pqZTS8v15+atklF4XGiMOtt7DnaDSuRrEPdZmdRUfO7eRRBe1PkA6/Uc+VtfOZRGd8QVvv9lHMXOC2FIMWCi1yE6icV5DCYHot56oIhEJZZJYVm29wkw1QBLiHxM5tUogoHz8/DBB04MUh6AHeSzMPmEFe1KHUd0kcfAGwqjxshO2JLQ65yupAgD+wkAuMGhk0idQSKOAA1iywHeJpEO/aDi3jdu+w3slMN+RsrivTkSPT8cdnetjNCqhEZV+VePCobjc1j4/LB6BZC5QfAD2Ct8hwbf+BRhaQm3COojBFo/jfCfoKewGlFDkk2KDjrpLWPdMxqRpCQdnWHkGDoy6YF3uDulHX8UxbWpGkAhQMLXXqM94nT3VvPwLpUCr3AW3M21JBxTqCa0bzoa7P6zWFOPCi4rQZmqhNsCFSKyjc2EtXKiT1BnCax/62n0Wu5KdYNtNXpVVJ8H+om1RG5XxGl3k06XRVAT7pdoMHXgMnVmi8gRrFCs/6gSOcPtsXWGZaGkzjcTSpmqZmE+huCYq77vXLbXL1+poCUAwDNuUGWfkXIwZLu3TBJpiv/mZpYlfdUhsOZvm68U3gxD3xgQalWu4vMhgNgebmlRwufZSU9s91C0d/QAyhNhgwzDl1ZO6R4LVlGSA0hKBfHmJSSqgTeaI0svjxP7YReDBFuCBZj5h84IhwJkuVDkv185Bb7Rh4IwgcnLCncOFhkeUHKjFUU+HptuCf17ARX4c22gizb84Biwd2qrpjJ0UiZ8J1n4WpZi2qrL/oRyTcEuZRWRFVPmfY7qvcaACT7frTtI3bDZuCDOiS8AOkAmHPXhUEMvkinXCAfXsktCUHJwZHPh/c/kEjhXDwXYQ2xjhnwM8w+kqakHxkRVt37Als5D5PftlqagFsA/J0i41skpq3dmGqKxzRh/QWkQL5T+cdDmfvjPccI+Uzg8EgZzBTO6yjusvc4j53IlW7dN0CPc5I7QXTE1SNbGzbIQg5xnmmUwdvZJZ3pN27H9xAdnwttHFI5kMoUwTkWRGoUiRtrpu7DUhVI2gSOF4HTtMoEngIbaVzs1mcK7Ns299qwdBg948dYIx6QdOCaX4CN3GeQZUOkDjSHH5ldq1sDRxNv1mkqlxNuJmNXeUmjAHja0/CS4fcYUGBnPztOgPstWEtj/Y6BpMoTd/I7BWZ4nRW5ogQgIsMdx2sO/XEnEBYGOadROf2aDcI/4iCQfusyZsQH2jQU+uVItFTq1cHXbcRAMMhetoQ+rsfqmG93zMvzamNCaZ00Ry0PAO9ysysWCbbpssXOytVnTHBwU8DoBkAELIROSELsJmhZEKkjpj1/Q7uVaabkpYt2Lol6JLaagnKBhOrcIIkQK64d2ssERIX8rdwyBRowAKRe3gM8rtKrxMLED4w4nxNsKtgBZj6tTplNmiGIaCkpy2MRzynEVwv+xBSZXS50MrVRsiyxKe/XlSYZTdOX5BHbKjaHW7fqhMZ5EZMLbPG8QsRQIiioBkYrsv51NG0JGAyjrGKT1UbfDlrtOXiK+2Nr+GuBF6DUiA2jFMOVR56sOFmadDzYMP6LdtY98PcBleB1QfzXswu00KJOyD5UA/KaRJp/+NqJ3/zDsW7q2AkKPVzx42eTnJ7nbWzQ5sgNtButtqm/7iudN6EwzNmLnt4xYdo6/zvzQpUKbabWhIXXDqEJmirN3lLc3bNYSeKULD0JS6nOFI664Tk3UegoIqXVuPiM1KIoiEAkv5/BMcUxWhLj8o8UPhc1QVyTvY3CWSHhTEB3+pp+a4MkqBJdpGpSOS82Vd1ig3YYGvPCBSZhdZk+D+HhTTYKJFRdkqXM+SN7fEwkd6Ex9k/r7wcYQ4F7svQ2aNk8Qv2OhtpZUPjTurbSxpRzo9fhT7kjIHtbG1Q3BGQjnKaFKada5TKHynouFltdf48Pi1hdZrLyLCyfkOV0qIKLaGyARZR+xsmhSj9xT8HzZ6rEaHhtVo2CICwj5jcdX2555Y0GKZ6pxpkeN9wWztJRV58eDxMLE2aSC4DHhLm8Ay+bAmEXJmFcxvltdNS1fPBzZRjgS+qandrUF5rKj7CR35BWWinWOfwi4jobOxOEexgHSlzYJEC62VU2PhyJeBssXA7wUzjRvP0TpWk+KWDpUVpe9wJuMVgqqJEPVDyOZ9y+QJYI0ukmxrL2MS0KtyhfT6J/d6zql3x6G/Xc8bIanNhDLzukWOWO6+WZumC7Wan/mPXn8FDtrEMS6EinQOhdeG3tWTbwJ0UQv3Tl2AA4aVoGOQJUq3alU5KvWz2Qbd5mtuKCXOPOaVCWR7nBkehi67+g+7UDnwYMLiXF2/QvXvn1uipP87Mh+IbY0GifVh0hVk08uE5nL+IGDTU17DA9QR3UI5PANPzr3fJLhf7gPQjXSeIrUU0bdz8Y9HS4teAHqjujWVzAWjDAw45hm+Wbv3cRuVKJtYAMDFB8mKrie5e8Dcn9xm8tCK4DntKRgoh40Xacr56X742NqDvW/L+JlEUnW+cLU8LoxFNsMFXFKeJJdFx6InVbwWbbXqzUxE/m0s0pZd41la60jhYsfUtXH9WiIL0Ljy8ZuGiE7wjyRqeVd7AK+IH/Ga8iiZvDLQ7RQ8AnOxTF+dj5Ae6Lg7mevGCpHyp9kZFRpBhwe5zPUUGy3WxGM30YvRhk16h54EFmCqBlLBSIzGcXgcjvO1lXcRWUqPVNLDSt9r86hgXHT8TAy5a52YYjhmEvtAFetG3bFmDbqKkFLKw8XsMuf7xhUoJdH89SUBZD2UJ8/Ljns3lB/D+xVAisFLYUkOfx0B/VjMkv4FzJCCJyTlw7k2/it9j7mlpW6+4g9ewCmFHRy3TwRlRPRGrk8dkAUM2ewTTLWgVw/72E3Sghths0sb6SogNQXs0K0fvWaDPbFQTewnCyf6x/QT99owPZ2S6Vfl9z9mqWwLunsZfBfPMHGYzpo0BsKXZZ74XUByq63unckCXwISIBe/Qe/DHWhFO+YnYYCg906awq+ydFZyACSbC2/WJ34GBHIm2UM+mX4h0o5XPNUiDpcG20+nT4igoOzX4JbjZ8wacAUBRRmZyJ6T1A2CKV7lNDdmBtDdVfSTXzlMgfFoAx23S8HkmiEgla9cOMzTbR3KvU/o8YY+suQ7mTr2bBY+X9mK7KWuHH3iDH/UKSAuWkFA+vYEo5vdqnsSRBw7OLb5exHgw4rEu5JmQCeiF2pV/eIn+DRTgGk/KccGSgNMrexOR3d5WrI3vLXLT+pAxubKDk/VDUWkof/qqEYtmncBx1roib3/ELsTIYCIAluSGjqHYUfKmiL32eiw8ICTghh21TFqPb459DA5atChfc9ayHiQp/IJi1vGTMFHhwFRuu2DAFeXFULJUnbuMdXLWfmMXGf8adN0laEnWWOXa5bmTjjfqRBMzMeU//2Of7AxS/5V0QysOQOTOmxAL2WLu0LYTO8Ayn1RoywnwcWRQZsh28OWaIhyIYawHnxWOi7lYVAd0VSP0VDkBoh5IvC7CMCGKeKI7dfiLYDLUp6JqteaMbSk4OG2FGM+nehPbZfo1rBNwl1q12nuB9Yyn4tUergJUpAqJpg8qTdy52842RGSvZq4g4y3xOC+FmiAp5P9F2NZRbReZPyYzLr6mzKl/YkGkeGTHDgn9sIkJTC36Eu6Vs/8hbnKAVtQCievn+2XQJHN4Zn7clph7KEZhhduFahYCmNaB7egak+mCiNP8cJIX2mQNtQ+u1TyzoDBLAVn7ogDBhHP1E+kNiIGDqvCq7RNz88DKSzBbyLME7+m0QC3AJWwwTBPptOsHqTZ7QfPb290CnVbMQ+w4XxpYGBiy26tTFdl6Cmk3oH53466P+rTEte30rinuwB/cLh/jvFjYBIbUGpX2QUowzeNzrL+NS1M922/Am/hHvTwQm52zVIS5w9XP4W+TDkCJTq/3tMc9c40oeu0xQayXNWjbC9Q7iC261Qt3uy06SCbk0sMNM2j/29tirSQMcmiJS8GAdkDulf4iEmuBhTpur3Dgd7kM8C/CWRqCxoluFhaZAY0BLVk7t3Q4QdjUMNeSnFCvNCmnsePw8WAYdn75OY8JiMKNxIxLQz9Ykg2YCXiOASi6y/qYB45FYaVPCyZgsH9G45gzMC4ZlbGjJm/0O0jdYt+bVjr/QiJD4YZLXOg2OIm+teAVqMsPIJPha2KdEbopUpTo3qRgBhOGwQCrYRsA8KHCLpFNTIlQN2Hy0EsYlzAZIA2yvP77n/GEUU/xCBX7uWxGkZbRovmZoxrEYgFemg6ra5dWRwBLf7W+BhYHcmIIPP3HjFgM8A8J5RLvjGOiEjFR9zIYpxlRtVSztb7vfOItXEuG0/DGLcMPfUevMALARVM+9eP5j4f4NhkdglrVDECr6jKX+GBIYF0nrPUs1qAOraOdUH/W2SoKYCnICCsXnh+mmtXJddforKQ0xqQsDy/6hXsk32fYJJCYTNRhR9QEZVw4nfEd/SLATyViBSUTEUiF3x33vHfaxZFQbmLESMEv5WNPaeABg5Rl9kJqQqX7uiP4iAQFLRELLwVOLlhTPt5Er3wpFn57XLhKbiVOwbQVYvblqDlVVOtqj4RClVKcUHN9Fl7FV22IVz7OnPSyHVgLXi4rwqVQylGeBLS5kpo8eAZ990Gi2UMkS36tr4U6KaTwkhZUTrGgDU3Cssj6SLrgv3CJvbeFuuL0nVcEhrpzYwZg1Pg+YdFB9nxhy+fsvjpJZJyiX7cIyxC/SRhVeysnLvm1/DPPLn0ycwAFLpxBqowQaTuP2k0J6xg7CR5XUdSA79/9G7qKS3A2gDUcny78NjBaw5fai5087N74q4SJZoAwMYnJz4gMBcsIi6GYq7uV/nQ5pd4+VAkfMemiJXj3yDpt4e9kmz/2yuBmDJSwh44H2Iir+fdg1ceJNANJ033SLeFA6QbbfmDFUGWp6lnyoQWvYeAlZtAR4mvhaXa8BmVqIYvcKNdUAlDf4/rdI+aaNygDK0zdrhr+nB4eKmP1xWy6Mmyy9pgZimdfHIX2RfahdXd+hiRz6JgQJjLjOZuwPTeA9szehR9ERoYENatl49Z3MTReSdhAQOillhJz7/+WoXzsZbBDqcFI+UxCldf2GfbjbgUTT2kgFt/EAivVACCwYRINxVQsX0RdqjEryLjldKdN8C6zpGEq2VcIUrjlwpWFhgNsNsk2VsM0qUT1d+T6/fJbzDytibbEkGbeK6NitztaywU+GNJ2ogGmHBlJWfTcxCrlRQnWBXSsIq7r76Cfh1z5DFBBc5gEh4A2JbZcLUhFUfFLYz6ezRDgy7Iso+SiQWjhQHRDvhbyAo1qyFpa5QwWbOSkisNspSR90ik2PdMlZEtviTNE7fEyXewGnmm4bHpkYH6yiBeKH2/k1+D+L0hIjDvdc8cgrOVpK4bEWE76xNNd0yWlZ0A9YxwLgKAC5SOACHb+RkX3LigkyCmajigTEF5gDARx1wd35d/kRA2cgGQAqE4yDpCHWGflMfW1qudRPLy/ME+AyqLF1Ifn3M6PPMtcz+jSbCqK6lpDzEnI9jvqkgDTV5m1kkZq9qY6Zom1R4g0M9hR31s/m3RbM0PUAokFPGJK7hslL0uv7odxHOtbNxofFsQ+WcpzrcEhOA5iifZ+UMrEU0spwuR9FSaPGBWl7X7KqF47ne5oTtJpAUyexgW1WDYPYTW8RP0q9UVnTwAN2qN7bi90IBWIWT/lBiYpAHDqqpyvPdbImLJqv0BZdf1YvovzCWg1iY7SiGQgK6E2VnkJfOPEAQWZzRkTHDNPiuk4sD6SHGDdAPx1FPb3nLntLBFkWyDuDh4cXlCi+OE/KyDkCmiaQZu0razEOdw5tzbVsIrWa0DK+SElkS/Zx7MHsZeW5XkrQFghiNUCYQeHRrXYlNetV9SgXjuPYaD462UnkLAwbIdcj4rXmm0UvGSCAhrxTg+kNT+w0irv+VPQgaX6kag6oEFhuM0ljF+P/iGvLSQwwMi9tce4O4lPTxhdjVQBSf+N6O97kU6tZD8NtnAz+SKtdrjzSCxImZig3faz8uWYgNdItopS0nw53oU8a3A5JOpx63HGQZRIsCJO/w5A5wjzdCsUfVdx+ICJTcuuJcouSBnlvKUTB5qtn9O9JztbImCNewhXttWLrG1H5sBEbhCRZFhmO6W1DaYco90XfGD872hfu8i7wdxaFOesxstPa5gI6vdvOtKuJl1mU37Piby2DStOVuCNB7S+lYu3CfFOOduaZJSyjQHOxQWpXtlIR9GJUY8TeQBwinPsTTpPG3kuZg33NZT0zmKNqef9tYFFwdSRY3i5Vk9ZMhu4BnfusGnHCYOxbZvgs/dOeFDmnaQrFM4yFXkBHtwFCfen/JFM8ma06V6GP9yQ9dTohAQGvvOViZuFPZMLUuLBwjlxmP1/ba78tStVhwXwqFruI6VaOlYpwC9dUUaTYZC8TxNMCiJVCsCZdfNXplZ39RUkp0I9UUhfVO5QwgPhFZfJoO2Q+g/3WY/oawrCUBJwTUmCB5kGnNoTcvC56VlvyWy0OBWO37ehs5pa4RVk4vi+jJPEgmCCWYCMA0iNycepGG4coK8KktegG4icVK5F4X+J1P3X8n1Xz9MI+54MwMJZU/i/bqAymTf3NXEv6wdp0ck0jmSFAgu/2TBqrT/+agIpT9WRBKcy8c4v/Oe1KAJtoEY0YRKMYcKBEKr4L2FWuf9Z5q6Oy+bZsJtinO3bYUHIRA5JajNda+cnURRy50imijOsFBxViIUJAVGs3VkemAM8QFPHU7Dat+0ngSe49glAA==");

  BloomFilter* bloom_filter = newBloomFilter(
      decodedBitmap.data(),
      static_cast<int32_t>(decodedBitmap.size()),
      7,
      13);

  for (int i = 0; i < TEST_SIZE; i++) {
    const std::string ithDocument = documentPrefix + std::to_string(i);
    EXPECT_EQ(expectedResults[i], mightContain(
        bloom_filter,
        ithDocument.c_str(),
        static_cast<int32_t>(ithDocument.length())));
  }

  deleteBloomFilter(bloom_filter);
}

}
