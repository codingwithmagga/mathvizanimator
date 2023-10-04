cmake_minimum_required(VERSION 3.24)

project(mva_gui VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Svg)

qt_policy(SET QTP0001 NEW)
qt_standard_project_setup()

qt_add_library(cwa.mva.gui
    STATIC
    include/private/logging.h

    src/logging.cpp
)

qt_add_qml_module(cwa.mva.gui
    URI cwa.mva.gui
    VERSION ${PROJECT_VERSION}
    QML_FILES
        qml/models/MVAItemModel.qml
        qml/models/MVAItemDelegate.qml
        qml/items/MVACircle.qml
        qml/items/MVARectangle.qml
        qml/items/MVAText.qml
        qml/items/MVAMouseArea.qml
        qml/items/MVASurroundingRectangle.qml
    SOURCES
        include/items/abstractitem.h
        include/items/circleitem.h
        include/items/rectangle.h
        include/items/textitem.h

        src/items/abstractitem.cpp
        src/items/circleitem.cpp
        src/items/rectangle.cpp
        src/items/textitem.cpp
    DEPENDENCIES QtQuick
    OUTPUT_DIRECTORY cwa/mva/gui
)

target_link_libraries(cwa.mva.gui
    PUBLIC
        Qt6::Quick
        Qt6::Svg
)

target_include_directories(cwa.mva.gui
    PUBLIC
        include/items
    PRIVATE
        include/private
)

add_library(mva::gui ALIAS cwa.mva.gui)
add_library(mva::guiplugin ALIAS cwa.mva.guiplugin)

# TODO: Needs to be changed for other systems (Windows, MAC)
configure_file(templates/template.tex $ENV{HOME}/.config/mathvizanimator/templates/template.tex)
configure_file(templates/test.svg $ENV{HOME}/.config/mathvizanimator/templates/test.svg)

add_subdirectory(tests)
