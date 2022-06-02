include(GoogleTest)

add_executable(unit_tests
    treap_tests.cpp
    )

gtest_discover_tests(unit_tests)

find_package(GTest REQUIRED CONFIG)

target_link_libraries(unit_tests
                        PRIVATE
                          algo_pack
                          GTest::gtest_main
                          GTest::gmock
                          project_warnings)
