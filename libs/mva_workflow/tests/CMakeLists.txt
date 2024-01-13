cmake_minimum_required(VERSION 3.24)

project(mva_tests VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Test Multimedia)

qt_standard_project_setup()

qt_add_executable(sfh_test_exe tst_savefilehandler.cpp)
add_test(NAME sfh_test_exe COMMAND sfh_test_exe)
target_link_libraries(sfh_test_exe
    PRIVATE
        Qt::Test
        mva::workflow
)

qt_add_executable(itemhandler_test_exe tst_itemhandler.cpp)
add_test(NAME itemhandler_test_exe COMMAND itemhandler_test_exe)
target_link_libraries(itemhandler_test_exe
    PRIVATE
        Qt::Test
        mva::workflow
)


qt_add_executable(renderer_test_exe tst_renderer.cpp
    renderer_test_resources.qrc)
add_test(NAME renderer_test_exe COMMAND renderer_test_exe)
target_link_libraries(renderer_test_exe
    PRIVATE
        Qt::Test
        mva::workflow
        Qt6::Multimedia
)
qt_add_resources(renderer_test_exe renderer_test_resources.qrc)

qt_add_executable(itemobserver_test_exe tst_itemobserver.cpp)
add_test(NAME itemobserver_test_exe COMMAND itemobserver_test_exe)
target_link_libraries(itemobserver_test_exe
    PRIVATE
        Qt::Test
        mva::workflow
)
