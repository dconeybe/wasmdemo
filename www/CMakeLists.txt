find_package (
  Python3
  "3.9...<4.0"
  REQUIRED
  COMPONENTS
    Interpreter
)

function(add_filtered_file ARG_TARGET_NAME)
  cmake_parse_arguments(
    PARSE_ARGV 0
    ARG
    "" # zero-value arguments
    "SRC;DEST" # single-value arguments
    "" # multi-value arguments
  )

  if("${ARG_TARGET_NAME}" STREQUAL "")
    message(FATAL_ERROR "TARGET_NAME must be specified to ${CMAKE_CURRENT_FUNCTION}")
  elseif("${ARG_SRC}" STREQUAL "")
    message(FATAL_ERROR "SRC must be specified to ${CMAKE_CURRENT_FUNCTION}")
  elseif("${ARG_DEST}" STREQUAL "")
    message(FATAL_ERROR "DEST must be specified to ${CMAKE_CURRENT_FUNCTION}")
  endif()

  add_custom_command(
    OUTPUT
      "${ARG_DEST}"
    COMMAND
      "${Python3_EXECUTABLE}"
      "${CMAKE_CURRENT_LIST_DIR}/filter.py"
      "${ARG_SRC}"
      "${ARG_DEST}"
      $<TARGET_FILE:wasmdemo>
    MAIN_DEPENDENCY
      "${ARG_SRC}"
    DEPENDS
      "${CMAKE_CURRENT_LIST_DIR}/filter.py"
      "${Python3_EXECUTABLE}"
      $<TARGET_FILE:wasmdemo>
    COMMENT
      "Generating ${ARG_DEST} from ${ARG_SRC}"
  )

  add_custom_target(
    "${ARG_TARGET_NAME}"
    ALL
    DEPENDS "${ARG_DEST}"
  )
endfunction(add_filtered_file)

add_filtered_file(
  generate_index_js
  SRC "${CMAKE_CURRENT_LIST_DIR}/index.js"
  DEST "${CMAKE_CURRENT_BINARY_DIR}/index.js"
)

add_filtered_file(
  generate_index_html
  SRC "${CMAKE_CURRENT_LIST_DIR}/index.html"
  DEST "${CMAKE_CURRENT_BINARY_DIR}/index.html"
)

add_filtered_file(
  generate_wasm_json
  SRC "${CMAKE_CURRENT_LIST_DIR}/wasm.json"
  DEST "${CMAKE_CURRENT_BINARY_DIR}/wasm.json"
)
