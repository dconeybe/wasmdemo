cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

message(STATUS "${CMAKE_CURRENT_LIST_FILE}: Using cmake version ${CMAKE_VERSION}: ${CMAKE_COMMAND}")

project(wasmdemo)

message(STATUS "${CMAKE_CURRENT_LIST_FILE}: CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")
message(STATUS "${CMAKE_CURRENT_LIST_FILE}: CMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set WASMDEMO_TARGET_WASM32 to YES if the build is targeting WebAssembly or
# NO if targeting any other platform (e.g. desktop).
if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "wasm32")
  set(WASMDEMO_TARGET_WASM32 YES)
elseif(WASMDEMO_CLANG_TARGET STREQUAL "native")
  set(WASMDEMO_TARGET_WASM32 NO)
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
if(WASMDEMO_TARGET_WASM32)
  add_compile_options(-fvisibility=hidden)
  add_compile_options($<$<CONFIG:Release>:-ffunction-sections>)
  add_compile_options($<$<CONFIG:Release>:-fdata-sections>)
  add_compile_options($<$<CONFIG:Release>:-flto>)
endif()

add_link_options(${WASMDEMO_WARNING_FLAGS})
if(WASMDEMO_TARGET_WASM32)
  add_link_options(-fvisibility=hidden)
  add_link_options($<$<CONFIG:Release>:-ffunction-sections>)
  add_link_options($<$<CONFIG:Release>:-fdata-sections>)
  add_link_options($<$<CONFIG:Release>:-flto>)
  add_link_options($<$<CONFIG:Release>:-Wl,--lto-O2>)
  add_link_options($<$<CONFIG:Release>:-Wl,--strip-all>)
  add_link_options($<$<CONFIG:Release>:-Wl,--gc-sections>)
endif()

add_subdirectory(cpp)

if(WASMDEMO_TARGET_WASM32)
  add_subdirectory(www)
else()
  add_subdirectory(external)
endif()
