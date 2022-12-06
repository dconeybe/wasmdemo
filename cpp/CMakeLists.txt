if(WASMDEMO_TARGET_WASM32)
  add_executable(wasmdemo src/wasmdemo.cc)
  set_property(TARGET wasmdemo PROPERTY OUTPUT_NAME wasmdemo.wasm)

  target_include_directories(
    wasmdemo
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/include/wasm32"
  )
else()
  add_library(wasmdemo src/wasmdemo.cc)

  target_include_directories(
    wasmdemo
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/include/nonwasm32"
  )
endif()
