find_package (
  Python3
  "3.9...<4.0"
  REQUIRED
  COMPONENTS
    Interpreter
)

function(
  dep_setup
  ARG_PACKAGE_NAME
  ARG_PACKAGE_VERSION
  ARG_DOWNLOAD_URL
  ARG_EXPECTED_SHA256
)
  if("${ARG_PACKAGE_NAME}" STREQUAL "")
    message(FATAL_ERROR "ARG_PACKAGE_NAME must be specified to ${CMAKE_CURRENT_FUNCTION}")
  endif()
  if("${ARG_PACKAGE_VERSION}" STREQUAL "")
    message(FATAL_ERROR "ARG_PACKAGE_VERSION must be specified to ${CMAKE_CURRENT_FUNCTION}")
  endif()
  if("${ARG_DOWNLOAD_URL}" STREQUAL "")
    message(FATAL_ERROR "ARG_DOWNLOAD_URL must be specified to ${CMAKE_CURRENT_FUNCTION}")
  endif()
  if("${ARG_EXPECTED_SHA256}" STREQUAL "")
    message(FATAL_ERROR "ARG_EXPECTED_SHA256 must be specified to ${CMAKE_CURRENT_FUNCTION}")
  endif()

  string(
    REPLACE
    "__PACKAGE_VERSION__"
    "${ARG_PACKAGE_VERSION}"
    ARG_DOWNLOAD_URL_FIXED
    "${ARG_DOWNLOAD_URL}"
  )

  set(
    PACKAGE_DEST_DIR
    "${CMAKE_CURRENT_LIST_DIR}/projects/${ARG_PACKAGE_NAME}"
  )

  set(
    DEP_SETUP_PY_ARGS
    "${Python3_EXECUTABLE}"
    "${CMAKE_CURRENT_LIST_DIR}/dep_setup.py"
    "${ARG_PACKAGE_NAME}"
    "${ARG_PACKAGE_VERSION}"
    "${ARG_DOWNLOAD_URL_FIXED}"
    "${ARG_EXPECTED_SHA256}"
    "${PACKAGE_DEST_DIR}"
  )

  string(JOIN " " DEP_SETUP_PY_ARGS_STR ${DEP_SETUP_PY_ARGS})
  message(
    STATUS
    "${CMAKE_CURRENT_LIST_FILE} ${CMAKE_CURRENT_FUNCTION}() "
    "running command: ${DEP_SETUP_PY_ARGS_STR}"
  )

  execute_process(
    COMMAND
      ${DEP_SETUP_PY_ARGS}
    RESULT_VARIABLE
      DEP_SETUP_PY_EXIT_CODE
  )

  if (NOT DEP_SETUP_PY_EXIT_CODE EQUAL 0)
    message(
      FATAL_ERROR
      "${CMAKE_CURRENT_FUNCTION} command failed with exit code "
      "${DEP_SETUP_PY_EXIT_CODE}: ${DEP_SETUP_PY_ARGS_STR}"
    )
  endif()

  add_subdirectory("${PACKAGE_DEST_DIR}")
endfunction(dep_setup)

if(WASMDEMO_TARGET_WASM32)
  set(
    GTEST_HAS_ABSL
    OFF
    CACHE
    BOOL
    "Tell googletest that absl and re2 are available."
  )
else()
  set(
    GTEST_HAS_ABSL
    ON
    CACHE
    BOOL
    "Tell googletest that absl and re2 are available."
  )

  set(
    ABSL_PROPAGATE_CXX_STD
    ON
    CACHE
    BOOL
    "Recommended by abseil since it will be the default in the future."
  )

  dep_setup(
    "abseil"
    "6dab0bd99c5856e747fe35e928395ae80ca5e5f1"
    "https://github.com/abseil/abseil-cpp/archive/__PACKAGE_VERSION__.zip"
    "57bffa111dac74fd4b0c7d4652a2a36cf0fd93c35b097b3a131f9414aaedc0b3"
  )

  dep_setup(
    "re2"
    "4be240789d5b322df9f02b7e19c8651f3ccbf205"
    "https://github.com/google/re2/archive/__PACKAGE_VERSION__.zip"
    "99ce27e7f11ab52776fd80daa2cae0d4045a021e93a03ed514951ba3d25f3982"
  )
endif()

set(
  INSTALL_GTEST
  OFF
  CACHE
  BOOL
  "Disable installation of googletest, since it's only embedded."
)

dep_setup(
  "googletest"
  "97459e54ec4f1882d219c413c929f719d9741ada"
  "https://github.com/google/googletest/archive/__PACKAGE_VERSION__.zip"
  "af17569ac5c05b148ae336a43ae7ab144d2cc5f3bfd3c9beb952cdd9a9fb0f75"
)
