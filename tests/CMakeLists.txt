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

target_link_libraries(unit_tests PRIVATE unit_test_essentials project_options project_warnings)

add_library(coverage_options INTERFACE)
# macro is defined in https://github.com/aminya/project_options internals
enable_coverage(coverage_options)

add_executable(coverage EXCLUDE_FROM_ALL ${ALPA_UNITTEST_FILES})
target_link_libraries(coverage PRIVATE unit_test_essentials coverage_options)
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
