option(BUILD_DOC "Build documentation" ON)
option(LIFT_DOCS "Lift documentation to folder build_docs" ON)

option(USE_SANITIZERS "Enable address/UB sanitizers" ON)

set(MY_PROJECT_VERSION_MAJOR ${CMAKE_PROJECT_VERSION_MAJOR})
set(MY_PROJECT_VERSION_MINOR ${CMAKE_PROJECT_VERSION_MINOR})
set(MY_PROJECT_VERSION_PATCH ${CMAKE_PROJECT_VERSION_PATCH})

message("${CMAKE_PROJECT_NAME} VERSION: ${CMAKE_PROJECT_VERSION}")
message("${CMAKE_PROJECT_NAME} PROJECT_DESCRIPTION: ${CMAKE_PROJECT_DESCRIPTION}")

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/config.h
)

FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (USE_SANITIZERS)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SANITIZER_FLAGS "-fsanitize=address,leak,undefined -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZER_FLAGS}")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ${SANITIZER_FLAGS}")
  endif()
endif()

message("\nCOMPILER USED: ${CMAKE_C_COMPILER} ${CMAKE_CXX_COMPILER}")
include(cmake/Flags.cmake)
message("\nFLAGS USED: ${CMAKE_CXX_FLAGS}")

find_program(CLANG_TIDY_EXE 
    NAMES clang-tidy-17 clang-tidy-16 clang-tidy-15 clang-tidy
    PATHS /usr/bin /usr/local/bin
)

# set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-p=build;--system-headers=false")
if(CLANG_TIDY_EXE)
    add_custom_target(
        tidy
        COMMAND ${CLANG_TIDY_EXE}
        -p ${CMAKE_BINARY_DIR}
        --system-headers=false
        ${ALL_CODE_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy"
    )
endif()

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
endif(WIN32)
