#include "wasmdemo/memory.h"

#include "gtest/gtest.h"

namespace {

using wasmdemo::memory::wasmdemo_malloc;
using wasmdemo::memory::wasmdemo_free;

TEST(memory, MallocAndFree) {
  void* buf = wasmdemo_malloc(1234);
  wasmdemo_free(buf);
}

}  // namespace