# Stop processing this file if the clang target is not WebAssembly.
# The "web site" created by the build targets below are only relevant when
# compiling to WebAssembly because it doesn't make any sense to embed any other
# build targets (e.g. x86_64, arm) into a web page.
if(NOT WASMDEMO_CLANG_TARGET_WASM32)
  return()
endif()

find_package (
  Python3
  "3.9...<4.0"
  REQUIRED
  COMPONENTS
    Interpreter
)

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
    "Generating ${CMAKE_CURRENT_BINARY_DIR}/index.js from ${CMAKE_CURRENT_LIST_DIR}/index.js"
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
    "Copying ${CMAKE_CURRENT_LIST_DIR}/index.html to ${CMAKE_CURRENT_BINARY_DIR}"
)

add_custom_target(
  copy_index_html
  ALL
  DEPENDS
    "${CMAKE_CURRENT_BINARY_DIR}/index.html"
)
