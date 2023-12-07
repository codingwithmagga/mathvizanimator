cmake_minimum_required(VERSION 3.24)

project(mva_gui_tests VERSION ${PROJECT_VERSION} LANGUAGES CXX C)

find_package(Qt6 6.5 REQUIRED COMPONENTS QuickTest)

qt_policy(SET QTP0001 NEW)
qt_standard_project_setup()

qt_add_executable(mainwindowhandler_test windows/test_mainwindowhandler.cpp)
qt_add_executable(qml_item_tests qml/main.cpp)
qt_add_executable(circleitem_test items/circleitem/tst_circleitem.cpp test_images.qrc)
qt_add_executable(rectangleitem_test items/rectangleitem/tst_rectangleitem.cpp test_images.qrc)
qt_add_executable(textitem_test items/textitem/tst_textitem.cpp test_images.qrc)

add_test(NAME mainwindowhandler_test COMMAND mainwindowhandler_test)
add_test(NAME qml_item_tests COMMAND qml_item_tests)
add_test(NAME circleitem_test COMMAND circleitem_test)
add_test(NAME rectangleitem_test COMMAND rectangleitem_test)
add_test(NAME textitem_test COMMAND textitem_test)

target_link_libraries(
    mainwindowhandler_test
    PRIVATE Qt6::QuickTest
    PRIVATE mva::gui
    PRIVATE mva::guiplugin
)

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

# Copy qml testfiles for qml_item_tests target to binary dir
file(COPY
        qml/tst_circleitem.qml
        qml/tst_rectangleitem.qml
        qml/tst_textitem.qml
    DESTINATION
        ${CMAKE_CURRENT_BINARY_DIR}
)
