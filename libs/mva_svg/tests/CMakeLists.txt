cmake_minimum_required(VERSION 3.24)

project(mva_svg_tests VERSION ${PROJECT_VERSION} LANGUAGES CXX C)

find_package(Qt6 6.5 REQUIRED COMPONENTS Test)

qt_standard_project_setup()

qt_add_executable(
    svgcreator_long_tests
        svgcreator_long_tests.cpp
        svg_test_helper_functions.h
        svg_test_resources.qrc
)
qt_add_executable(
    latexprocess_long_tests
        latexprocess_long_tests.cpp
        svg_test_helper_functions.h
        svg_test_resources.qrc
)
qt_add_executable(
    dvisvgmprocess_long_tests
        dvisvgmprocess_long_tests.cpp
        svg_test_helper_functions.h
        svg_test_resources.qrc
)

add_test(NAME svgcreator_long_tests COMMAND svgcreator_long_tests)
add_test(NAME latexprocess_long_tests COMMAND latexprocess_long_tests)
add_test(NAME dvisvgmprocess_long_tests COMMAND dvisvgmprocess_long_tests)

set_tests_properties(
    svgcreator_long_tests
    latexprocess_long_tests
    dvisvgmprocess_long_tests
    PROPERTIES
        LABELS LONG
)

target_link_libraries(
    svgcreator_long_tests
    PRIVATE
        Qt6::Test
        mva::svg
)
target_link_libraries(
    latexprocess_long_tests
    PRIVATE
        Qt6::Test
        mva::svg
)
target_link_libraries(
    dvisvgmprocess_long_tests
    PRIVATE
        Qt6::Test
        mva::svg
)
