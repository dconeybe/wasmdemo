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
add_link_options($<$<CONFIG:Release>:-ffunction-sections>)
add_link_options($<$<CONFIG:Release>:-fdata-sections>)
add_link_options($<$<CONFIG:Release>:-flto>)
add_link_options($<$<CONFIG:Release>:-Wl,--lto-O2>)
add_link_options($<$<CONFIG:Release>:-Wl,--strip-all>)
add_link_options($<$<CONFIG:Release>:-Wl,--gc-sections>)

find_package (Python3 COMPONENTS Interpreter)
if(NOT Python3_FOUND)
  message(FATAL_ERROR "Unable to find a Python3 interpreter")
endif()

add_executable(wasmdemo wasmdemo.cc)
set_property(TARGET wasmdemo PROPERTY OUTPUT_NAME wasmdemo.wasm)

add_custom_command(
  OUTPUT
    "${CMAKE_CURRENT_BINARY_DIR}/index.js"
  COMMAND
    "${Python3_EXECUTABLE}"
    "${CMAKE_CURRENT_LIST_DIR}/generate_index_js.py"
    index.js
    "${CMAKE_CURRENT_LIST_DIR}/index.js"
    $<TARGET_FILE:wasmdemo>
  MAIN_DEPENDENCY
    "${CMAKE_CURRENT_LIST_DIR}/index.js"
  DEPENDS
    "${CMAKE_CURRENT_LIST_DIR}/generate_index_js.py"
    "${Python3_EXECUTABLE}"
    $<TARGET_FILE:wasmdemo>
  COMMENT
    "Generating ${CMAKE_CURRENT_BINARY_DIR}/index.js"
)

add_custom_target(
  generate_index_js
  ALL
  DEPENDS
    "${CMAKE_CURRENT_BINARY_DIR}/index.js"
)

add_custom_command(
  OUTPUT
    "${CMAKE_CURRENT_BINARY_DIR}/index.html"
  COMMAND
    "${CMAKE_COMMAND}"
    -E
    copy
    "${CMAKE_CURRENT_LIST_DIR}/index.html"
    "${CMAKE_CURRENT_BINARY_DIR}/index.html"
  MAIN_DEPENDENCY
    "${CMAKE_CURRENT_LIST_DIR}/index.html"
  COMMENT
    "Copying index.html to ${CMAKE_CURRENT_BINARY_DIR}"
)

add_custom_target(
  copy_index_html
  ALL
  DEPENDS
    "${CMAKE_CURRENT_BINARY_DIR}/index.html"
)
