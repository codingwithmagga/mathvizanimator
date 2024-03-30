cmake_minimum_required(VERSION 3.24)

project(mva_svg_tests VERSION ${PROJECT_VERSION} LANGUAGES CXX C)

find_package(Qt6 6.5 REQUIRED COMPONENTS Test)

qt_standard_project_setup()

qt_add_executable(
    svg_creator_long_tests
    svg_creator_long_tests.cpp
        svg_test_helper_functions.h
        svg_test_resources.qrc
)
qt_add_executable(
    latex_process_long_tests
    latex_process_long_tests.cpp
        svg_test_helper_functions.h
        svg_test_resources.qrc
)
qt_add_executable(
    dvisvgm_process_long_tests
    dvisvgm_process_long_tests.cpp
        svg_test_helper_functions.h
        svg_test_resources.qrc
)

add_test(NAME svg_creator_long_tests COMMAND svg_creator_long_tests)
add_test(NAME latex_process_long_tests COMMAND latex_process_long_tests)
add_test(NAME dvisvgm_process_long_tests COMMAND dvisvgm_process_long_tests)

set_tests_properties(
    svg_creator_long_tests
    latex_process_long_tests
    dvisvgm_process_long_tests
    PROPERTIES
        LABELS LONG
)

target_link_libraries(
    svg_creator_long_tests
    PRIVATE
        Qt6::Test
        mva::svg
)
target_link_libraries(
    latex_process_long_tests
    PRIVATE
        Qt6::Test
        mva::svg
)
target_link_libraries(
    dvisvgm_process_long_tests
    PRIVATE
        Qt6::Test
        mva::svg
)
