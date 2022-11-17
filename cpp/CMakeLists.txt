configure_file(
  "${CMAKE_CURRENT_LIST_DIR}/config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/include/wasmdemo/config.h"
)

if(WASMDEMO_CLANG_TARGET_WASM32)
  add_executable(wasmdemo src/wasmdemo.cc)
  target_link_options(wasmdemo PRIVATE -Wl,--no-entry)
  set_property(TARGET wasmdemo PROPERTY OUTPUT_NAME wasmdemo.wasm)
else()
  add_library(wasmdemo src/wasmdemo.cc)
endif()

target_include_directories(
  wasmdemo
  PRIVATE
  "${CMAKE_CURRENT_BINARY_DIR}/include"
  "${CMAKE_CURRENT_LIST_DIR}/include"
)
