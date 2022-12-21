###############################################################################
# wasmdemo_lib
###############################################################################

add_library(
  wasmdemo_lib
  OBJECT
  src/wasmdemo.cc
  src/hash.cc
  src/bloom.cc
)

target_compile_options(
  wasmdemo_lib
  PRIVATE
  -Werror
)

target_link_options(
  wasmdemo_lib
  PRIVATE
  -Werror
)

target_include_directories(
  wasmdemo_lib
  PUBLIC
  "${CMAKE_CURRENT_LIST_DIR}/include/common"
)

if(WASMDEMO_TARGET_WASM32)
  target_include_directories(
    wasmdemo_lib
    PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/include/wasm32"
  )
else()
  target_include_directories(
    wasmdemo_lib
    PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/include/nonwasm32"
  )
endif()

###############################################################################
# wasmdemo browser binary
###############################################################################

if(WASMDEMO_TARGET_WASM32)
  add_executable(wasmdemo src/wasmdemo_main.cc)
  set_property(TARGET wasmdemo PROPERTY OUTPUT_NAME wasmdemo.wasm)
  target_link_libraries(
    wasmdemo
    PUBLIC
    wasmdemo_lib
  )
  target_link_options(
    wasmdemo
    PUBLIC
    "-nostartfiles"
    "-Wl,--no-entry"
  )
endif()

###############################################################################
# wasmdemo_lib unit tests
###############################################################################

if(BUILD_TESTING)
  add_executable(
    wasmdemo_test
    test/hash_test.cc
    test/wasmdemo_imports_impl.cc
    test/wasmdemo_test.cc
  )

  target_include_directories(
    wasmdemo_test
    PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/test"
  )

  target_link_libraries(
    wasmdemo_test
    PRIVATE
    wasmdemo_lib
    gmock_main
  )

  add_test(
    NAME wasmdemo_test
    COMMAND wasmdemo_test
  )
endif()
