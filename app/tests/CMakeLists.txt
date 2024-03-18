cmake_minimum_required(VERSION 3.24)

project(mva_app_tests VERSION ${PROJECT_VERSION} LANGUAGES CXX C)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Test Quick Multimedia)

qt_policy(SET QTP0001 NEW)
qt_standard_project_setup()

qt_add_executable(int_menu_file_test
    integration_tests/tst_menu_file.cpp
    integration_tests/test_helper_functions.h
    integration_tests/test_helper_functions.cpp
    integration_tests_data/integrations_tests_res.qrc
)
qt_add_executable(int_menu_project_test
    integration_tests/tst_menu_project.cpp
    integration_tests/test_helper_functions.h
    integration_tests/test_helper_functions.cpp
)
qt_add_executable(int_animation_test
    integration_tests/tst_animation.cpp
    integration_tests/test_helper_functions.h
    integration_tests/test_helper_functions.cpp
)
qt_add_executable(int_user_interaction_test
    integration_tests/tst_user_interaction.cpp
    integration_tests/test_helper_functions.h
    integration_tests/test_helper_functions.cpp
)

add_test(NAME int_menu_file_test COMMAND int_menu_file_test)
add_test(NAME int_menu_project_test COMMAND int_menu_project_test)
add_test(NAME int_animation_test COMMAND int_animation_test)
add_test(NAME int_user_interaction_test COMMAND int_user_interaction_test)

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
target_link_libraries(
    int_animation_test
    PRIVATE Qt::Test
    PRIVATE Qt6::Quick
    PRIVATE mva::applib
)
target_link_libraries(
    int_user_interaction_test
    PRIVATE Qt::Test
    PRIVATE Qt6::Quick
    PRIVATE mva::applib
)
