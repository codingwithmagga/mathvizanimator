cmake_minimum_required(VERSION 3.24)

project(mathvizanimator-documentation VERSION ${PROJECT_VERSION} LANGUAGES CXX)

find_package(Doxygen REQUIRED)

# Find all the public headers
get_target_property(MVA_GUI_PUBLIC_HEADER_DIRS mva::gui INTERFACE_INCLUDE_DIRECTORIES)

foreach(INCLUDE_DIR ${MVA_GUI_PUBLIC_HEADER_DIRS})
  file(GLOB_RECURSE INCLUDE_DIR_PUBLIC_HEADERS ${INCLUDE_DIR}/*.h)
  list(APPEND MVA_GUI_PUBLIC_HEADERS ${INCLUDE_DIR_PUBLIC_HEADERS})
endforeach()

set(DOXYGEN_INPUT_DIR ${CMAKE_SOURCE_DIR}/libs)
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs/doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

#Replace variables inside @@ with the current values
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR}) #Doxygen won't create this for us
add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
                   DEPENDS ${MVA_GUI_PUBLIC_HEADERS}
                   COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                   MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
                   COMMENT "Generating docs")

add_custom_target(doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})
