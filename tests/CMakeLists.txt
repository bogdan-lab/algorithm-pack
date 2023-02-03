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
    # Building INCLUDE WHAT YOU USE target
    add_custom_target(iwyu)
    set(IWYU_COMMAND_LINE_ARGS "")
    # Accumulate includes
    get_target_property(ALPA_INCLUDES algo_pack INTERFACE_INCLUDE_DIRECTORIES)
    foreach(INCLUDE_PATH ${ALPA_INCLUDES})
        list(APPEND IWYU_COMMAND_LINE_ARGS -I ${INCLUDE_PATH})
    endforeach()
    
    add_standard_include_pathes()
    foreach(EL_PATH ${CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES})
        list(APPEND IWYU_COMMAND_LINE_ARGS -isystem ${EL_PATH})
    endforeach()

    # Accumulate other iwyu options
    list(APPEND IWYU_COMMAND_LINE_ARGS 
                    -Xiwyu --mapping_file=${CMAKE_CURRENT_SOURCE_DIR}/iwyu_mapping.imp)
    foreach(INCLUDE_PATH ${ALPA_INCLUDES})
        list(APPEND IWYU_COMMAND_LINE_ARGS  -Xiwyu --check_also=${INCLUDE_PATH}/*.h)
    endforeach()
    
    find_program(INCLUDE_WHAT_YOU_USE include-what-you-use)
    set(IWYU_COMMAND_CHAIN "")
    if(INCLUDE_WHAT_YOU_USE)
        foreach(TEST_FILE ${ALPA_UNITTEST_FILES})
            list(APPEND IWYU_COMMAND_CHAIN 
                        "COMMAND" ${INCLUDE_WHAT_YOU_USE}
                        ${IWYU_COMMAND_LINE_ARGS} ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_FILE} || "true")
        endforeach()
    else()
        list(APPEND IWYU_COMMAND_CHAIN "COMMAND" echo "include-what-you-use was not found!")
    endif()

    # Run include-what-you-use
    add_custom_command(TARGET iwyu POST_BUILD
                       COMMENT "IWYU run for all"
                       ${IWYU_COMMAND_CHAIN} VERBATIM)
else()
    message(STATUS "`iwyu` target is valid only for builds with ALPA_STATIC_ANALYSIS option. Otherwise target is excluded entirely.")
endif()