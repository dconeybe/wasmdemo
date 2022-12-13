cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

message(STATUS "${CMAKE_CURRENT_LIST_FILE}: Using cmake version ${CMAKE_VERSION}: ${CMAKE_COMMAND}")

project(wasmdemo)

message(STATUS "${CMAKE_CURRENT_LIST_FILE}: CMAKE_COMMAND=${CMAKE_COMMAND}")
message(STATUS "${CMAKE_CURRENT_LIST_FILE}: CMAKE_VERSION=${CMAKE_VERSION}")
message(STATUS "${CMAKE_CURRENT_LIST_FILE}: CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "${CMAKE_CURRENT_LIST_FILE}: CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")
message(STATUS "${CMAKE_CURRENT_LIST_FILE}: CMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "${CMAKE_CURRENT_LIST_FILE}: CMAKE_GENERATOR=${CMAKE_GENERATOR}")
message(STATUS "${CMAKE_CURRENT_LIST_FILE}: CMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}")
message(STATUS "${CMAKE_CURRENT_LIST_FILE}: CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "${CMAKE_CURRENT_LIST_FILE}: CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
message(STATUS "${CMAKE_CURRENT_LIST_FILE}: BUILD_TESTING=${BUILD_TESTING}")
if(BUILD_TESTING)
  enable_testing()

  # Enable official support for tests with whitespace and other special
  # characters in the name specified to add_test().
  # https://cmake.org/cmake/help/v3.19/policy/CMP0110.html
  cmake_policy(SET CMP0110 NEW)
endif()

if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "wasm32")
  set(WASMDEMO_TARGET_WASM32 YES)
else()
  set(WASMDEMO_TARGET_WASM32 NO)
endif()

set(WASMDEMO_WARNING_FLAGS
  -Wall
  -Wextra
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
if(WASMDEMO_TARGET_WASM32)
  add_compile_options($<$<CONFIG:Release>:-ffunction-sections>)
  add_compile_options($<$<CONFIG:Release>:-fdata-sections>)
  add_compile_options($<$<CONFIG:Release>:-flto>)
endif()

add_link_options(${WASMDEMO_WARNING_FLAGS})
if(WASMDEMO_TARGET_WASM32)
  add_link_options($<$<CONFIG:Release>:-ffunction-sections>)
  add_link_options($<$<CONFIG:Release>:-fdata-sections>)
  add_link_options($<$<CONFIG:Release>:-flto>)
  add_link_options($<$<CONFIG:Release>:-Wl,--lto-O2>)
  add_link_options($<$<CONFIG:Release>:-Wl,--strip-all>)
  add_link_options($<$<CONFIG:Release>:-Wl,--gc-sections>)
endif()

if(WASMDEMO_TARGET_WASM32)
  add_compile_definitions(GTEST_HAS_EXCEPTIONS=0)
  add_compile_definitions(GTEST_HAS_STREAM_REDIRECTION=0)
  set(gtest_disable_pthreads YES CACHE BOOL "Disable pthreads in googletest" FORCE)
endif()

add_subdirectory(cpp)
add_subdirectory(external)

if(WASMDEMO_TARGET_WASM32)
  add_subdirectory(www)
endif()
