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

function (get_all_cmake_tests out_var current_dir)
    get_property(targets DIRECTORY ${current_dir} PROPERTY TESTS)
    get_property(subdirs DIRECTORY ${current_dir} PROPERTY SUBDIRECTORIES)

    foreach(subdir ${subdirs})
        get_all_cmake_tests(subdir_targets ${subdir})
        list(APPEND targets ${subdir_targets})
    endforeach()

    set(${out_var} ${targets} PARENT_SCOPE)
endfunction()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTORCC TRUE)
set(CMAKE_AUTOUIC TRUE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(BUILD_DOCUMENTATION CACHE BOOL OFF)
set(RUN_CODE_COVERAGE CACHE BOOL OFF)

# For QtCreator to recognize the qml gui lib
set(QML_IMPORT_PATH ${CMAKE_BINARY_DIR}/libs/mva_gui CACHE STRING "" FORCE)

if (MSVC)
    # Wanted to use /Wall, but only after the problem below is solved
    enable_cxx_compiler_flag_if_supported("/W4")
    # The flag "/WX" treats warnings as errors. Unfortunately not all warnings from Qt headers are suppressed using the external flags below.
    # So currently this flag is disabled. Maybe someone finds a solution for this. The Qt headers are included as /external:I in github actions
    # so this seems to work correctly.
    #enable_cxx_compiler_flag_if_supported("/WX")
    enable_cxx_compiler_flag_if_supported("/external:anglebrackets")
    enable_cxx_compiler_flag_if_supported("/external:W0")
    enable_cxx_compiler_flag_if_supported("/external:templates-")
else()
    enable_cxx_compiler_flag_if_supported("-Wall")
    enable_cxx_compiler_flag_if_supported("-Werror")
    enable_cxx_compiler_flag_if_supported("-Wextra")
    enable_cxx_compiler_flag_if_supported("-pedantic")
endif()

if(CMAKE_COMPILER_IS_GNUCC AND RUN_CODE_COVERAGE)
    include(CheckCCompilerFlag)
    include(CodeCoverage)
    append_coverage_compiler_flags()
    enable_cxx_compiler_flag_if_supported("-fno-exceptions")
endif()
