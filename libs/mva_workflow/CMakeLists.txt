include(CheckCXXCompilerFlag)

function(enable_cxx_compiler_flag_if_supported flag)
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if(flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if(flag_supported)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
        endif()
        unset(flag_supported CACHE)
    endif()
endfunction()

cmake_minimum_required(VERSION 3.16)

project(mva_workflow VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_library(workflow
    SHARED
    src/savefilehandler.cpp
    src/logging.cpp

    include/savefilehandler.h
)
add_library(mva::workflow ALIAS workflow)

target_link_libraries(workflow
    PRIVATE
        Qt6::Core
)

target_include_directories(workflow
    PUBLIC
        include
)

enable_cxx_compiler_flag_if_supported("-Wall")
enable_cxx_compiler_flag_if_supported("-Wextra")
enable_cxx_compiler_flag_if_supported("-pedantic")

# Showing header files in Qt Creator project view
FILE(GLOB_RECURSE HeaderFiles "*.h")
add_custom_target(workflow_headers SOURCES ${HeaderFiles})
