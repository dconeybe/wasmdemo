cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(wasmdemo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

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

configure_file(
  "${CMAKE_CURRENT_LIST_DIR}/config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/include/wasmdemo/config.h"
)

add_compile_options(-Wall -Wextra -Werror)
if(WASMDEMO_CLANG_TARGET_WASM32)
  add_compile_options(--target=wasm32)
  add_compile_options(-nostdlib)
  add_compile_options(-fvisibility=hidden)
  add_compile_options($<$<CONFIG:Release>:-ffunction-sections>)
  add_compile_options($<$<CONFIG:Release>:-fdata-sections>)
  add_compile_options($<$<CONFIG:Release>:-flto>)
endif()

add_link_options(-Wall -Wextra -Werror)
if(WASMDEMO_CLANG_TARGET_WASM32)
  add_link_options(--target=wasm32)
  add_link_options(-nostdlib)
  add_link_options(-fvisibility=hidden)
  add_link_options(-Wl,--no-entry)
  add_link_options($<$<CONFIG:Release>:-ffunction-sections>)
  add_link_options($<$<CONFIG:Release>:-fdata-sections>)
  add_link_options($<$<CONFIG:Release>:-flto>)
  add_link_options($<$<CONFIG:Release>:-Wl,--lto-O2>)
  add_link_options($<$<CONFIG:Release>:-Wl,--strip-all>)
  add_link_options($<$<CONFIG:Release>:-Wl,--gc-sections>)
endif()

if(WASMDEMO_CLANG_TARGET_WASM32)
  add_executable(wasmdemo src/wasmdemo.cc)
  set_property(TARGET wasmdemo PROPERTY OUTPUT_NAME wasmdemo.wasm)
else()
  add_library(wasmdemo src/wasmdemo.cc)
endif()

target_include_directories(
  wasmdemo
  PRIVATE
  "${CMAKE_CURRENT_BINARY_DIR}/include"
  PUBLIC
  "${CMAKE_CURRENT_LIST_DIR}/include"
)

add_subdirectory(www)
