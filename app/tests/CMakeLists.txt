cmake_minimum_required(VERSION 3.24)

project(mva_app_tests VERSION ${PROJECT_VERSION} LANGUAGES CXX C)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Test Quick Multimedia)

qt_policy(SET QTP0001 NEW)
qt_standard_project_setup()

qt_add_executable(int_menu_file_test
    integration_tests/tst_menu_file.cpp
    integration_tests/inttesthelperfcts.h
    integration_tests/inttesthelperfcts.cpp
    integration_tests_data/integrations_tests_res.qrc
)
qt_add_executable(int_menu_project_test
    integration_tests/tst_menu_project.cpp
    integration_tests/inttesthelperfcts.h
    integration_tests/inttesthelperfcts.cpp
)

add_test(NAME int_menu_file_test COMMAND int_menu_file_test)
add_test(NAME int_menu_project_test COMMAND int_menu_project_test)

target_link_libraries(
    int_menu_file_test
    PRIVATE Qt::Test
    PRIVATE Qt6::Quick
    PRIVATE mva::applib
)
target_link_libraries(
    int_menu_project_test
    PRIVATE Qt::Test
    PRIVATE Qt6::Quick
    PRIVATE mva::applib
    PRIVATE Qt6::Multimedia
)
