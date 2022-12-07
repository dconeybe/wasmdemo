set(
  wasmdemo_srcs
  src/wasmdemo.cc
  src/hash.cc
)

if(WASMDEMO_TARGET_WASM32)
  add_executable(wasmdemo ${wasmdemo_srcs})
  set_property(TARGET wasmdemo PROPERTY OUTPUT_NAME wasmdemo.wasm)

  target_include_directories(
    wasmdemo
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/include/wasm32"
  )
else()
  add_library(wasmdemo ${wasmdemo_srcs})

  target_include_directories(
    wasmdemo
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/include/nonwasm32"
  )
endif()

target_compile_options(
  wasmdemo
  PRIVATE
  -Werror
)

target_link_options(
  wasmdemo
  PRIVATE
  -Werror
)
