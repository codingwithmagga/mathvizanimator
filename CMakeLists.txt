cmake_minimum_required(VERSION 3.24)

set(PROJECT_VERSION 0.0.1)
project(mathvizanimator VERSION ${PROJECT_VERSION} LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
message("Code cover: ${RUN_CODE_COVERAGE}")
include(presets)

enable_testing(true)

add_subdirectory(libs)
add_subdirectory(src)

get_all_cmake_tests(TEST_TARGETS ${CMAKE_CURRENT_LIST_DIR})

if(CMAKE_COMPILER_IS_GNUCC AND RUN_CODE_COVERAGE)
    set(GCOVR_ADDITIONAL_ARGS "--exclude-unreachable-branches")
    setup_target_for_coverage_gcovr_xml(
        NAME mva_test_coverage
        EXECUTABLE ctest --schedule-random -j 4
        DEPENDENCIES ${TEST_TARGETS}
        EXCLUDE libs/mva_gui/tests/* libs/mva_workflow/tests/*
    )
endif()
