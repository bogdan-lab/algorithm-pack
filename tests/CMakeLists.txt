add_executable(unit_tests
    treap_tests.cpp
    )


target_include_directories(unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    )

target_link_libraries(unit_tests PRIVATE gtest_main gmock)
