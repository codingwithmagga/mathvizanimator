cmake_minimum_required(VERSION 3.16)

set(PROJECT_VERSION 0.0.1)

project(mathvizanimator VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

add_subdirectory(libs/mva_workflow)
add_subdirectory(src)
add_subdirectory(tests)
