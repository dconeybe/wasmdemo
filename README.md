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

Clang doesn't often ship pre-installed with wasm32 target support. Installing
llvm from homebrew, however, will provide a clang with wasm32 support. It can
be easily installed by running `brew install llvm`.

## Build Instructions

The build process uses a normal cmake build process. The only real difference is
to explictly specify the path to clang that supports wasm32.

To generate the build files, run:

```
CXX=/home/dconeybe/local/homebrew/Cellar/llvm/15.0.4/bin/clang++ cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

Then to build, and rebuild, run

```
cmake --build build
```

This will generate `build/www/index.html`, which can be opened in a web browser
to exercise the compiled C++ code.