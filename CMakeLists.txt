cmake_minimum_required(VERSION 3.16)

set(PROJECT_VERSION 0.0.1)
project(mathvizanimator VERSION ${PROJECT_VERSION} LANGUAGES CXX)

include(cmake/presets.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(libs)
add_subdirectory(src)
