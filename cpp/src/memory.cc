#include "wasmdemo/memory.h"

#include <cstdlib>

namespace wasmdemo {
namespace memory {

WASM_EXPORT("malloc")
void* wasmdemo_malloc(int32_t size) {
  if (size < 0) {
    abort();
  }
  void* buf = malloc(static_cast<size_t>(size));
  if (! buf) {
    abort();
  }
  return buf;
}

WASM_EXPORT("free")
void wasmdemo_free(void* ptr) {
  free(ptr);
}

}  // namespace memory
}  // namespace wasmdemo
