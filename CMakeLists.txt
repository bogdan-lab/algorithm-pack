cmake_minimum_required(VERSION 3.19)

project(AlgorithmPack LANGUAGES CXX)

set(GTEST_PATH third-party/gtest-submodule)
add_subdirectory(${GTEST_PATH} ${GTEST_PATH}/build)

add_subdirectory(tests)
