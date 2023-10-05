cmake_minimum_required(VERSION 3.24)

project(mva_tests VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Test Multimedia)

qt_standard_project_setup()

qt_add_executable(sfh_test_exe tst_savefilehandler.cpp)
add_test(NAME sfh_test COMMAND sfh_test_exe)
target_link_libraries(sfh_test_exe
    PRIVATE
        Qt::Test
        mva::workflow
)

qt_add_executable(renderer_test_exe tst_renderer.cpp)
add_test(NAME renderer_test COMMAND renderer_test_exe)
target_link_libraries(renderer_test_exe
    PRIVATE
        Qt::Test
        mva::workflow
        Qt6::Multimedia
)
