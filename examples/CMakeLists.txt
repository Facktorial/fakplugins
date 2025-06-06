cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)

file(GLOB EXAMPLE_SOURCES "*.cpp")

######################################
set(PLUGIN_LIBRARIES spdlog::spdlog)
#set(PLUGIN_LIBRARIES fakstd::fakstd spdlog::spdlog)
add_library(rendererplugin SHARED ${EXAMPLES_DIR}/RendererPlugin.cpp)
set_target_properties(rendererplugin PROPERTIES PREFIX "")
target_link_libraries(rendererplugin PUBLIC ${PLUGIN_LIBRARIES})
target_include_directories(rendererplugin PUBLIC ${INCLUDE_DIR})

add_library(rendererplugin2 SHARED ${EXAMPLES_DIR}/RendererPlugin2.cpp)
set_target_properties(rendererplugin2 PROPERTIES PREFIX "")
target_link_libraries(rendererplugin2 PUBLIC ${PLUGIN_LIBRARIES})
target_include_directories(rendererplugin2 PUBLIC ${INCLUDE_DIR})

add_library(rendererplugin3 SHARED ${EXAMPLES_DIR}/RendererPlugin3.cpp)
set_target_properties(rendererplugin3 PROPERTIES PREFIX "")
target_link_libraries(rendererplugin3 PUBLIC ${PLUGIN_LIBRARIES})
target_include_directories(rendererplugin3 PUBLIC ${INCLUDE_DIR})
######################################

# foreach(example_source ${EXAMPLE_SOURCES})
#     get_filename_component(example_name ${example_source} NAME_WE)
#     
#     add_executable(${example_name} ${example_source})
#     message("creating binary: ${example_name}")
#     target_link_libraries(${example_name} PUBLIC dl fakstd::fakstd)
# endforeach()

add_executable(two_plugins ${EXAMPLES_DIR}/2plugins.cpp)

# Link libraries for dynamic loading
if(WIN32)
    # Windows doesn't need additional libraries for LoadLibrary
else()
  target_link_libraries(two_plugins PUBLIC fakplugins dl)
  #target_link_libraries(two_plugins PUBLIC fakplugins dl fakstd::fakstd)
endif()

target_include_directories(two_plugins PRIVATE ${INCLUDE_DIR})

# Copy plugins to build directory
add_custom_command(TARGET two_plugins POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rendererplugin> $<TARGET_FILE_DIR:two_plugins>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rendererplugin2> $<TARGET_FILE_DIR:two_plugins>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rendererplugin3> $<TARGET_FILE_DIR:two_plugins>
)
