#ifndef WASMDEMO_CPP_INCLUDE_COMMON_WASMDEMO_MEMORY_H_
#define WASMDEMO_CPP_INCLUDE_COMMON_WASMDEMO_MEMORY_H_

#include <cstdint>

#include "wasmdemo/macros.h"

namespace wasmdemo {
namespace memory {

WASM_EXPORT("malloc")
void* wasmdemo_malloc(int32_t);

WASM_EXPORT("free")
void wasmdemo_free(void*);

}  // namespace memory
}  // namespace wasmdemo

#endif  // WASMDEMO_CPP_INCLUDE_COMMON_WASMDEMO_MEMORY_H_