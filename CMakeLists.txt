cmake_minimum_required(VERSION 3.10)

project(
    fakplugins
    VERSION 0.0.1
    DESCRIPTION "My custom standard template library for support my gamedev."
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(PLUGIN_MANAGER_BUILD_EXAMPLES "Build example plugins" ON)

set(CMAKE_DIR "${CMAKE_SOURCE_DIR}/cmake")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(EXAMPLES_DIR "${CMAKE_SOURCE_DIR}/examples")

include_directories(${CMAKE_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})
include_directories(${EXAMPLES_DIR})

file(GLOB_RECURSE ALL_CODE_FILES 
    ${INCLUDE_DIR}/*.cpp
    ${INCLUDE_DIR}/*.h
    ${SOURCE_DIR}/*.cpp
    ${SOURCE_DIR}/*.h
    ${EXAMPLES_DIR}/*.cpp
    ${EXAMPLES_DIR}/*.h
)

include(cmake/IntroProject.cmake)

#find_package(fakstd REQUIRED)
find_package(spdlog REQUIRED)

add_library("${CMAKE_PROJECT_NAME}" STATIC ${SOURCE_DIR}/Plugins.cpp)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
#target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC fakstd::fakstd spdlog::spdlog)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC spdlog::spdlog)

add_executable("${CMAKE_PROJECT_NAME}_bin"
  ${SOURCE_DIR}/main.cpp
)
target_link_libraries("${CMAKE_PROJECT_NAME}_bin" PRIVATE
  #${CMAKE_PROJECT_NAME} fakstd::fakstd spdlog::spdlog
  ${CMAKE_PROJECT_NAME} spdlog::spdlog
)

add_library(fakplugins::fakplugins ALIAS fakplugins)

add_subdirectory(${EXAMPLES_DIR})

IF (BUILD_DOC)
message("Building docs")
  include(cmake/Doxygen.cmake)
  enable_doxygen()
ENDIF (BUILD_DOC)
