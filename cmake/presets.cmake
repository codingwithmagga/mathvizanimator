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

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# For QtCreator to recognize the qml gui lib
set(QML_IMPORT_PATH ${CMAKE_BINARY_DIR}/libs/mva_gui CACHE STRING "" FORCE)

if (MSVC)
    enable_cxx_compiler_flag_if_supported("/Wall")
    enable_cxx_compiler_flag_if_supported("/WX")
    enable_cxx_compiler_flag_if_supported("/external:anglebrackets")
    enable_cxx_compiler_flag_if_supported("/external:W0")

    # TODO: Find a way to make this path independent
    enable_cxx_compiler_flag_if_supported('/external:I "D:\\a\\mathvizanimator\\mathvizanimator\\Qt\\6.5.1\\msvc2019_64\\include" ')
else()
    enable_cxx_compiler_flag_if_supported("-Wall")
    enable_cxx_compiler_flag_if_supported("-Werror")
    enable_cxx_compiler_flag_if_supported("-Wextra")
    enable_cxx_compiler_flag_if_supported("-pedantic")
endif()
