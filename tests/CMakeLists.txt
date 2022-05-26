add_executable(unit_tests
    treap_tests.cpp
    )

find_package(GTest REQUIRED CONFIG)

target_include_directories(unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    )

target_link_libraries(unit_tests PRIVATE GTest::gtest_main GTest::gmock)
