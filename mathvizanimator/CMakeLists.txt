cmake_minimum_required(VERSION 3.16)

project(mathvizanimator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)
find_package(Cairo REQUIRED)

pkg_check_modules(CairoMM REQUIRED IMPORTED_TARGET cairomm-1.0)

qt_standard_project_setup()

qt_add_executable(mathvizanimator-app
    src/main.cpp
    src/mainwindow.cpp
    src/rectangle.cpp
    src/abstractobject.cpp
    src/objectmodel.cpp
    src/qmlimageprovider.cpp
)

qt_add_qml_module(mathvizanimator-app
    URI mathvizanimator
    VERSION 1.0
    QML_FILES src/Main.qml src/ItemModel.qml
    DEPENDENCIES QtQuick
)

set_target_properties(mathvizanimator-app PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(mathvizanimator-app
    PRIVATE
        Qt6::Quick
        Cairo::Cairo
        PkgConfig::CairoMM
)

target_include_directories(mathvizanimator-app
    PRIVATE
        src
)

install(TARGETS mathvizanimator-app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Showing header files in Qt Creator project view
FILE(GLOB_RECURSE HeaderFiles "src/*.h")
add_custom_target(headers SOURCES ${HeaderFiles})
