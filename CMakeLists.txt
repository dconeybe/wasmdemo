cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(wasmdemo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

# By default, build the wasm32 target.
# But also allow building for the native platform, to enable running the
# unit tests.
set(WASMDEMO_CLANG_TARGET "wasm32" CACHE STRING
  "The target architecture to build for. Must be either 'wasm32' or 'native'."
)
if(WASMDEMO_CLANG_TARGET STREQUAL "wasm32")
  set(WASMDEMO_CLANG_TARGET_WASM32 YES)
  set(WASMDEMO_CLANG_TARGET_NATIVE NO)
elseif(WASMDEMO_CLANG_TARGET STREQUAL "native")
  set(WASMDEMO_CLANG_TARGET_WASM32 NO)
  set(WASMDEMO_CLANG_TARGET_NATIVE YES)
else()
  message(FATAL_ERROR "Invalid value for WASMDEMO_CLANG_TARGET: ${WASMDEMO_CLANG_TARGET}")
endif()

set(WASMDEMO_WARNING_FLAGS
  -Wall
  -Wextra
  -Werror
  -Wformat
  -Wuninitialized
  -fno-common
  -Wunused-function
  -Wunused-value
  -Wunused-variable
  -Wreorder
  -Wshadow
  -Wconversion
  -Wpedantic
)

add_compile_options(${WASMDEMO_WARNING_FLAGS})
if(WASMDEMO_CLANG_TARGET_WASM32)
  add_compile_options(--target=wasm32)
  add_compile_options(-nostdlib)
  add_compile_options(-fvisibility=hidden)
  add_compile_options($<$<CONFIG:Release>:-ffunction-sections>)
  add_compile_options($<$<CONFIG:Release>:-fdata-sections>)
  add_compile_options($<$<CONFIG:Release>:-flto>)
endif()

add_link_options(${WASMDEMO_WARNING_FLAGS})
if(WASMDEMO_CLANG_TARGET_WASM32)
  add_link_options(--target=wasm32)
  add_link_options(-nostdlib)
  add_link_options(-fvisibility=hidden)
  add_link_options($<$<CONFIG:Release>:-ffunction-sections>)
  add_link_options($<$<CONFIG:Release>:-fdata-sections>)
  add_link_options($<$<CONFIG:Release>:-flto>)
  add_link_options($<$<CONFIG:Release>:-Wl,--lto-O2>)
  add_link_options($<$<CONFIG:Release>:-Wl,--strip-all>)
  add_link_options($<$<CONFIG:Release>:-Wl,--gc-sections>)
endif()

add_subdirectory(cpp)
add_subdirectory(www)
