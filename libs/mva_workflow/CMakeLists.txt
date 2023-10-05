cmake_minimum_required(VERSION 3.24)

project(mva_workflow VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_library(workflow
    SHARED
    src/savefilehandler.cpp
    src/logging.cpp

    include/savefilehandler.h
)
add_library(mva::workflow ALIAS workflow)

target_link_libraries(workflow
    PUBLIC
        Qt6::Core
)

target_include_directories(workflow
    PUBLIC
        include
)

add_subdirectory(tests)
