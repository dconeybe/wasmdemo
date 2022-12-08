cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

if(NOT DEFINED WASM32_CLANG_ROOT)
  if(NOT DEFINED ENV{WASM32_CLANG_ROOT})
    message(FATAL_ERROR "WASM32_CLANG_ROOT must be set")
  endif()
  set(WASM32_CLANG_ROOT "$ENV{WASM32_CLANG_ROOT}")
endif()
if(NOT IS_DIRECTORY "${WASM32_CLANG_ROOT}")
  message(FATAL_ERROR "WASM32_CLANG_ROOT is not a directory: ${WASM32_CLANG_ROOT}")
endif()

if(DEFINED WASI_SYSROOT)
  set(CMAKE_SYSROOT "${WASI_SYSROOT}")
elseif(DEFINED ENV{WASI_SYSROOT})
  set(CMAKE_SYSROOT "$ENV{WASI_SYSROOT}")
endif()

# Use "Generic" for CMAKE_SYSTEM_NAME to avoid the spurious "System is unknown
# to CMake" warning (https://stackoverflow.com/q/24907916)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR wasm32)

set(CMAKE_C_COMPILER "${WASM32_CLANG_ROOT}/bin/clang")
set(CMAKE_CXX_COMPILER "${WASM32_CLANG_ROOT}/bin/clang++")
set(CMAKE_AR "${WASM32_CLANG_ROOT}/bin/llvm-ar")
set(CMAKE_RANLIB "${WASM32_CLANG_ROOT}/bin/llvm-ranlib")
set(CMAKE_C_COMPILER_TARGET wasm32-wasi)
set(CMAKE_CXX_COMPILER_TARGET wasm32-wasi)

# Don't look in the sysroot for executables to run during the build
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Only look in the sysroot (not in the host paths) for the rest
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_C_FLAGS_INIT "")
set(CMAKE_CXX_FLAGS_INIT "-fno-exceptions -fno-rtti")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-nostartfiles -Wl,--no-entry -fno-exceptions -fno-rtti")
