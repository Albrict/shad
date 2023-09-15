cmake_minimum_required(VERSION 3.14.0) # FetchContent is available in 3.11+
project(shad
  DESCRIPTION "Terminal graphical editor"
  LANGUAGES C)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined")

# Notcurses dependency 
add_subdirectory(libs/notcurses)
# Third party libraries
include(libs/libs.cmake)
# Our Project
include(src/src.cmake)

add_executable(${PROJECT_NAME} ${libs} ${shad_source})

target_link_libraries(${PROJECT_NAME} notcurses)
