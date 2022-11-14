cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(wasmdemo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_compile_options(-Wall -Wextra -Werror)
add_compile_options(--target=wasm32)
add_compile_options(-nostdlib)
add_compile_options(-fvisibility=hidden)
add_compile_options($<$<CONFIG:Release>:-ffunction-sections>)
add_compile_options($<$<CONFIG:Release>:-fdata-sections>)
add_compile_options($<$<CONFIG:Release>:-flto>)

add_link_options(-Wall -Wextra -Werror)
add_link_options(--target=wasm32)
add_link_options(-nostdlib)
add_link_options(-fvisibility=hidden)
add_link_options(-Wl,--no-entry)
add_link_options(-Wl,--export-dynamic)
add_link_options($<$<CONFIG:Release>:-ffunction-sections>)
add_link_options($<$<CONFIG:Release>:-fdata-sections>)
add_link_options($<$<CONFIG:Release>:-flto>)
add_link_options($<$<CONFIG:Release>:-Wl,--lto-O2>)
add_link_options($<$<CONFIG:Release>:-Wl,--strip-all>)
add_link_options($<$<CONFIG:Release>:-Wl,--gc-sections>)

add_executable(wasmdemo wasmdemo.cc)
