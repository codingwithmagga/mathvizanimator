cmake_minimum_required(VERSION 3.24)

project(mva_svg VERSION ${PROJECT_VERSION} LANGUAGES CXX C)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Svg)

qt_standard_project_setup()

qt_add_library(cwa.mva.svg
    STATIC

    include/svgcreator.h
    include/latexprocess.h
    include/dvisvgmprocess.h
    include/svg_config.h

    src/svgcreator.cpp
    src/latexprocess.cpp
    src/dvisvgmprocess.cpp
)

target_link_libraries(cwa.mva.svg
    PUBLIC
        Qt6::Core
        Qt6::Svg
)

target_include_directories(cwa.mva.svg
    PUBLIC
        include
)

add_library(mva::svg ALIAS cwa.mva.svg)

add_subdirectory(tests)
