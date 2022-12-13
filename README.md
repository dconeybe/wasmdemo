# WebAssembly Firestore Demo

This repository builds a tiny web site that includes a component written in C++
and compiled to WebAssembly.

## Prerequisites

The following software must be installed in order to build the project:

* cmake 3.22 or newer
* ninja build tool.
* clang and llvm with wasm32 support
* wasi-sdk sysroot
* wasmtime

cmake, ninja, and wasmtime can be installed in many ways. On macOS and Linux, I
just use homebrew (i.e. `brew install cmake ninja wasmtime`). cmake and ninja
can also be installed using Python's package manager
(i.e. `pip install cmake ninja`).

Operating Systems like macOS and Ubuntu often don't ship with a clang that
includes wasm32 support. So you set up a clang that _does_ have wasm32 support.
There are many ways to do this, but two of them are highlighted here:

1. Install a precompiled wasi-sdk and use its clang.
2. Install clang via the homebrew package manager.

### Using a precompiled wasi-sdk

A precompiled wasi-sdk can be downloaded from
https://github.com/WebAssembly/wasi-sdk/releases. Simply pick the .tar.gz file
for your OS (e.g. `wasi-sdk-17.0-linux.tar.gz` or `wasi-sdk-17.0-macos.tar.gz`)
and extract it to a directory on your computer.

### Using clang from the homebrew package manager.

#### Install clang using `brew install`.

Clang can easily be installed in homebrew by running `brew install llvm`.
Afterwards, run `brew info llvm` to find the directory into which it was
installed. Later on, when running cmake, set the `WASM32_CLANG_ROOT` environment
variable to this installation directory.

For example, when I run it on my computer, this is what I see:

```
$ brew info llvm
==> llvm: stable 15.0.5 (bottled), HEAD [keg-only]
Next-gen compiler infrastructure
https://llvm.org/
/Users/dconeybe/local/homebrew/Cellar/llvm/15.0.4 (6,412 files, 1.3GB)
  Poured from bottle on 2022-11-13 at 00:07:11
```

And I will use

```
WASM32_CLANG_ROOT=/Users/dconeybe/local/homebrew/Cellar/llvm/15.0.4 cmake ...
```

#### Install the wasi-sysroot.

The wasi-sysroot must be downloaded separately from
https://github.com/WebAssembly/wasi-sdk/releases. At the time of writing, the
`latest version is `wasi-sysroot-17.0.tar.gz`. Extract this file anywhere on
your computer. The path will be specified to cmake as the `WASI_SYSROOT`
environment variable.

#### Install the `libclang_rt.builtins-wasm32.a` in homebrew's clang.

The `libclang_rt.builtins-wasm32.a` must be downloaded separately from
https://github.com/WebAssembly/wasi-sdk/releases. At the time of writing, the
`latest version is `libclang_rt.builtins-wasm32-wasi-17.0.tar.gz`.

Extract the `.tar.gz` file anywhere on your computer. It will extract a single
file: `lib/wasi/libclang_rt.builtins-wasm32.a`. Later on, the first time that
you run the `cmake` command, you will see an error like this:

```
wasm-ld: error: cannot open .../.../wasi/libclang_rt.builtins-wasm32.a: No such file or directory
```

Simply copy the extracted `libclang_rt.builtins-wasm32.a` to the path mentioned
in the error message, creating any missing directories.

## Build Instructions

The build process uses a normal cmake build process. The only real difference is
to explictly specify the path to the clang toolchain that supports wasm32, and
specify the cmake "toolchain" file.

If using the wasi-sdk clang toolchain, generate the build files by running:

```
WASM32_CLANG_ROOT=.../wasi-sdk-17.0 \
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_TOOLCHAIN_FILE=wasm32.toolchain.cmake
```

If using clang from homebrew, run the same command but also set the
`WASI_SYSROOT` environment variable:

```
WASM32_CLANG_ROOT=.../wasi-sdk-17.0 \
WASI_SYSROOT=.../wasi-sysroot \
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_TOOLCHAIN_FILE=wasm32.toolchain.cmake
```

Then to build, and rebuild, run

```
cmake --build build
```

This will generate `build/www/index.html`, which can be opened in a web browser
to exercise the compiled C++ code.
