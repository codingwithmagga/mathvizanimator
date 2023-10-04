cmake_minimum_required(VERSION 3.24)

project(mva_gui_tests VERSION ${PROJECT_VERSION} LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS QuickTest)

qt_policy(SET QTP0001 NEW)
qt_standard_project_setup()

enable_testing(true)

qt_add_executable(item_tests main.cpp)

add_test(NAME item_tests COMMAND item_tests)

target_link_libraries(
    item_tests
    PRIVATE Qt6::QuickTest
    PRIVATE Qt6::Qml
    PRIVATE mva::gui
    PRIVATE mva::guiplugin
)

# Copy qml test files to build folder. I'm not sure if this is the right way to do it.
configure_file(qml/tst_circleitem.qml tst_circleitem.qml)
configure_file(qml/tst_rectangleitem.qml tst_rectangleitem.qml)
configure_file(qml/tst_textitem.qml tst_textitem.qml)
