add_executable(unit_tests
    unit_tests.cpp
    treap_tests.cpp
    )


target_include_directories(unit_tests PRIVATE
    ${GTET_PATH}/googletest
    ${GTEST_PATH}/googlemock
    ${CMAKE_SOURCE_DIR}/include
    )

target_link_libraries(unit_tests PRIVATE gtest gmock)
