cmake_minimum_required(VERSION 3.16)

project(mathvizanimator-app VERSION ${PROJECT_VERSION} LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Svg)

qt_policy(SET QTP0001 NEW)
qt_standard_project_setup()

qt_add_executable(mva-app
    main.cpp
    gui/mainwindow.cpp
    models/objectmodel.cpp
)

qt_add_qml_module(mva-app
    URI cwa.mva
    VERSION ${PROJECT_VERSION}
    QML_FILES
        gui/Main.qml
        models/MVAItemModel.qml
        models/MVAItemDelegate.qml
        items/MVACircle.qml
        items/MVARectangle.qml
        items/MVAText.qml
        items/MVAMouseArea.qml
        items/MVASurroundingRectangle.qml
    SOURCES
        items/abstractitem.cpp
        items/circleitem.cpp
        items/rectangle.cpp
        items/textitem.cpp
    DEPENDENCIES QtQuick
)

set_target_properties(mva-app PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(mva-app
    PRIVATE
        Qt6::Quick
        Qt6::Svg
)

target_include_directories(mva-app
    PRIVATE
        items
        models
        gui
)

install(TARGETS mva-app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Showing header files in Qt Creator project view
FILE(GLOB_RECURSE HeaderFiles "*.h")
add_custom_target(headers SOURCES ${HeaderFiles})
