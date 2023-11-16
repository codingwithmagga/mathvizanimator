cmake_minimum_required(VERSION 3.24)

project(mathvizanimator-app VERSION ${PROJECT_VERSION} LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Svg)

qt_policy(SET QTP0001 NEW)
qt_standard_project_setup()

qt_add_executable(mva-app
    src/main.cpp
    src/gui/mainwindow.cpp
    src/gui/mainwindow.h
    src/workflow/logging.cpp
    src/workflow/logging.h
)

qt_add_qml_module(mva-app
    URI cwa.mva.app
    VERSION ${PROJECT_VERSION}
    QML_FILES
        src/qml/Main.qml
    DEPENDENCIES QtQuick
)

set_target_properties(mva-app PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER cwa.mva.app
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(mva-app
    PRIVATE
        mva::gui
        mva::guiplugin
        mva::workflow
)

target_include_directories(mva-app
    PRIVATE
        src/items
        src/gui
        src/workflow
)

install(TARGETS mva-app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
