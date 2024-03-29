cmake_minimum_required(VERSION 3.24)

project(mva_gui VERSION ${PROJECT_VERSION} LANGUAGES CXX C)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Quick Svg)

qt_policy(SET QTP0001 NEW)
qt_standard_project_setup()

qt_add_library(cwa.mva.gui
    STATIC
    include/windows/mainwindowhandler.h
    include/animations/abstract_animation.h
    include/animations/property_animation.h
    include/animations/real_property_animation.h
    include/animations/fade_in.h
    include/animations/fade_out.h

    src/windows/mainwindowhandler.cpp
    src/animations/abstract_animation.cpp
    src/animations/property_animation.cpp
    src/animations/real_property_animation.cpp
    src/animations/fade_in.cpp
    src/animations/fade_out.cpp
)

qt_add_qml_module(cwa.mva.gui
    URI cwa.mva.gui
    VERSION ${PROJECT_VERSION}
    QML_FILES
        qml/MainWindow.qml
        qml/models/MVAItemModel.qml
        qml/models/MVAItemDelegate.qml
        qml/items/MVACircle.qml
        qml/items/MVARectangle.qml
        qml/items/MVAText.qml
        qml/items/MVAMouseArea.qml
        qml/items/MVASurroundingRectangle.qml
        qml/windows/TextDialog.qml
    SOURCES
        include/items/abstractitem.h
        include/items/circleitem.h
        include/items/rectangle.h
        include/items/textitem.h
        include/items/basic_item.h
        include/items/geometry_item.h

        src/items/abstractitem.cpp
        src/items/circleitem.cpp
        src/items/rectangle.cpp
        src/items/textitem.cpp
        src/items/basic_item.cpp
        src/items/geometry_item.cpp
    DEPENDENCIES QtQuick
    OUTPUT_DIRECTORY cwa/mva/gui
    RESOURCES mva_gui_resources.qrc
)

target_link_libraries(cwa.mva.gui
    PUBLIC
        Qt6::Core
        Qt6::Quick
        Qt6::Svg
)

target_include_directories(cwa.mva.gui
    PUBLIC
        include/items
        include/windows
        include/animations
)

add_library(mva::gui ALIAS cwa.mva.gui)
add_library(mva::guiplugin ALIAS cwa.mva.guiplugin)

# TODO(codingwithmagga): Add these files to a user path, so that they can be changed by the user
# Currently they are provided by a resource file
# configure_file(templates/template.tex $ENV{HOME}/.config/mathvizanimator/templates/template.tex)
# configure_file(templates/placeholder.svg $ENV{HOME}/.config/mathvizanimator/templates/placeholder.svg)

add_subdirectory(tests)
