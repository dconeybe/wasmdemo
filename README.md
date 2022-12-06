# WebAssembly Firestore Demo

This repository builds a tiny web site that includes a component written in C++
and compiled to WebAssembly.

## Prerequisites

The following software must be installed in order to build the project:

* cmake 3.22 or newer
* ninja build tool.
* clang and llvm with wasm32 support

Cmake and Ninja can be installed in many ways. On macOS and Linux, I just use
homebrew (i.e. `brew install cmake ninja`). It can also be installed using
Python's package manager (i.e. `pip install cmake ninja`).

Operating Systems like macOS and Ubuntu often don't ship with a clang that
includes wasm32 support. So you set up a clang that _does_ have wasm32 support.
There are many ways to do this, but two of them are highlighted here:

1. Install a precompiled wasi-sdk and use its clang.
2. Install clang via the homebrew package manager.

A precompiled wasi-sdk can be downloaded from
https://github.com/WebAssembly/wasi-sdk/releases. Simply pick the .tar.gz file
for your OS (e.g. `wasi-sdk-16.0-linux.tar.gz` or `wasi-sdk-16.0-macos.tar.gz`)
and extract it to a directory on your computer.

Clang can easily be installed in homebrew by running `brew install llvm`.

## Build Instructions

The build process uses a normal cmake build process. The only real difference is
to explictly specify the path to the clang toolchain that supports wasm32, and
specify the cmake "toolchain" file.

To generate the build files, run:

```
WASM32_CLANG_ROOT=$PWD/wasi-sdk-16.0 \
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_TOOLCHAIN_FILE=wasm32.toolchain.cmake
```

Obviously, replace `$PWD/wasi-sdk-16.0` with the path of the directory
containing `bin/clang`, `bin/clang++`, etc.

Then to build, and rebuild, run

```
cmake --build build
```

This will generate `build/www/index.html`, which can be opened in a web browser
to exercise the compiled C++ code.
