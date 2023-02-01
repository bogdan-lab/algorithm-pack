include(GoogleTest)

set(ALPA_UNITTEST_FILES 
    treap_tests.cpp
    implicit_treap_tests.cpp
)

add_executable(unit_tests ${ALPA_UNITTEST_FILES})

gtest_discover_tests(unit_tests)

find_package(GTest REQUIRED CONFIG)

add_library(unit_test_essentials INTERFACE)
target_link_libraries(unit_test_essentials INTERFACE algo_pack GTest::gtest_main GTest::gmock)

target_link_libraries(unit_tests PRIVATE unit_test_essentials project_options)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_library(coverage_options INTERFACE)
    # macro is defined in https://github.com/aminya/project_options internals
    enable_coverage(coverage_options)

    add_executable(coverage EXCLUDE_FROM_ALL ${ALPA_UNITTEST_FILES})
    target_link_libraries(coverage PRIVATE unit_test_essentials coverage_options)

    if(ALPA_STATIC_ANALYSIS)
        # macro is defined in https://github.com/aminya/project_options internals
        target_disable_static_analysis(coverage)
    endif()

    add_custom_command(TARGET coverage
                       POST_BUILD
                       COMMENT "Run all tests and capture coverage"
                       COMMAND ./coverage
                       COMMAND lcov -q --capture --directory . --output-file=all_coverage.info
                       COMMAND lcov -q --extract all_coverage.info "${CMAKE_SOURCE_DIR}/*" --output-file=coverage.info
                       COMMAND rm all_coverage.info
                       COMMAND genhtml  coverage.info --output-directory=coverage_html
                       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                       # VERBATIM is required in order to correct lcov --extract command processing
                       VERBATIM)
else()
    message(STATUS "`coverage` target is valid only for Debug build. Otherwise target is excluded entirely.")
endif()


if(ALPA_STATIC_ANALYSIS)
    add_custom_target(iwyu)

    set(IWYU_COMMAND_LINE_ARGS "")
    get_target_property(ALPA_INCLUDES algo_pack INTERFACE_INCLUDE_DIRECTORIES)
    foreach(INCLUDE_PATH ${ALPA_INCLUDES})
        list(APPEND IWYU_COMMAND_LINE_ARGS -I ${INCLUDE_PATH})
    endforeach()
    
    message(STATUS "ALPA include directories = ${ALPA_INCLUDES}")
    get_target_property(GTEST_INCLUDES GTest::gtest_main INCLUDE_DIRECTORIES)
    message(STATUS "GTEST include directories = ${GTEST_INCLUDES}")
    get_target_property(GMOCK_INCLUDES GTest::gmock INCLUDE_DIRECTORIES)
    message(STATUS "GMOCK include directories = ${GMOCK_INCLUDES}")

    message(STATUS "CXX_DEFINES = ${CXX_DEFINES}")
    message(STATUS "CXX_INCLUDES = ${CXX_INCLUDES}")
    message(STATUS "CXX_FLAGS = ${CXX_FLAGS}")


    add_standard_include_pathes()
    foreach(EL_PATH ${CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES})
        message(STATUS "EL_PATH = ${EL_PATH}")
        list(APPEND IWYU_COMMAND_LINE_ARGS -isystem ${EL_PATH})
    endforeach()


    list(APPEND IWYU_COMMAND_LINE_ARGS -Xiwyu --verbose=3)

    message(STATUS "COMMAND LINE = ${IWYU_COMMAND_LINE_ARGS}")

    set(CHAIN_COMMANDS "")
    foreach(TEST_FILE ${ALPA_UNITTEST_FILES})
        list(APPEND CHAIN_COMMANDS include-what-you-use ${IWYU_COMMAND_LINE_ARGS} ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_FILE} &&)
    endforeach()
    
    list(APPEND CHAIN_COMMANDS echo "All files are checked")

    add_custom_command(TARGET iwyu
                       POST_BUILD
                       COMMENT "IWYU run for all"
                       COMMAND ${CHAIN_COMMANDS}
                       VERBATIM)
    # add_executable(iwyu EXCLUDE_FROM_ALL ${ALPA_UNITTEST_FILES})
    # target_link_libraries(iwyu PRIVATE unit_test_essentials)
    # target_enable_include_what_you_use(iwyu)
else()
    message(STATUS "`iwyu` target is valid only for builds with ALPA_STATIC_ANALYSIS option. Otherwise target is excluded entirely.")
endif()