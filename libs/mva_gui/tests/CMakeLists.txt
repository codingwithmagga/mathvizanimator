cmake_minimum_required(VERSION 3.24)

project(mva_gui_tests VERSION ${PROJECT_VERSION} LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS QuickTest)

qt_policy(SET QTP0001 NEW)
qt_standard_project_setup()

qt_add_executable(qml_item_tests qml/main.cpp)
qt_add_executable(circleitem_test items/tst_circleitem.cpp)
qt_add_executable(rectangleitem_test items/tst_rectangleitem.cpp)
qt_add_executable(textitem_test items/tst_textitem.cpp)

add_test(NAME qml_item_tests COMMAND qml_item_tests)
add_test(NAME circleitem_test COMMAND circleitem_test)
add_test(NAME rectangleitem_test COMMAND rectangleitem_test)
add_test(NAME textitem_test COMMAND textitem_test)

target_link_libraries(
    qml_item_tests
    PRIVATE Qt6::QuickTest
    PRIVATE Qt6::Qml
    PRIVATE mva::gui
    PRIVATE mva::guiplugin
)

target_link_libraries(
    circleitem_test
    PRIVATE Qt6::QuickTest
    PRIVATE Qt6::Qml
    PRIVATE mva::gui
    PRIVATE mva::guiplugin
)

target_link_libraries(
    rectangleitem_test
    PRIVATE Qt6::QuickTest
    PRIVATE Qt6::Qml
    PRIVATE mva::gui
    PRIVATE mva::guiplugin
)

target_link_libraries(
    textitem_test
    PRIVATE Qt6::QuickTest
    PRIVATE Qt6::Qml
    PRIVATE mva::gui
    PRIVATE mva::guiplugin
)

# Copy qml test files to build folder. I'm not sure if this is the right way to do it.
configure_file(qml/tst_circleitem.qml tst_circleitem.qml COPYONLY)
configure_file(qml/tst_rectangleitem.qml tst_rectangleitem.qml COPYONLY)
configure_file(qml/tst_textitem.qml tst_textitem.qml COPYONLY)
