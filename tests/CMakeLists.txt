include(GoogleTest)

add_executable(unit_tests
    treap_tests.cpp
    implicit_treap_tests.cpp
    )

gtest_discover_tests(unit_tests)

find_package(GTest REQUIRED CONFIG)

target_link_libraries(unit_tests
                        PRIVATE
                          algo_pack
                          GTest::gtest_main
                          GTest::gmock)



add_custom_command(TARGET unit_tests
                   POST_BUILD
                   COMMENT "Run all tests and capture coverage"
                   COMMAND ctest --output-on-failure
                   COMMAND lcov -q --capture --directory . --output-file=all_coverage.info
                   COMMAND lcov -q --extract all_coverage.info "${CMAKE_SOURCE_DIR}/*" --output-file=coverage.info
                   COMMAND rm all_coverage.info
                   COMMAND genhtml  coverage.info --output-directory=coverage
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   # VERBATIM is required in order to correct lcov --extract command processing
                   VERBATIM)
