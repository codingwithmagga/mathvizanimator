cmake_minimum_required(VERSION 3.16)

project(mva_gui_project VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Svg)

qt_policy(SET QTP0001 NEW)
qt_standard_project_setup()

qt_add_library(mva_gui STATIC)

qt_add_qml_module(mva_gui
    URI "mva_gui"
    VERSION 0.1
    QML_FILES
        models/MVAItemModel.qml
        models/MVAItemDelegate.qml
        items/MVACircle.qml
        items/MVARectangle.qml
        items/MVAText.qml
    SOURCES
        items/abstractitem.cpp
        items/circleitem.cpp
        items/rectangle.cpp
        items/textitem.cpp
    DEPENDENCIES QtQuick
)

target_link_libraries(mva_gui
    PUBLIC
        Qt6::Quick
        Qt6::Svg
)

target_include_directories(mva_gui
    PUBLIC
        items
        models
)
