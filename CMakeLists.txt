cmake_minimum_required(VERSION 3.19)

project(AlgorithmPack LANGUAGES CXX)

add_library(algo_pack INTERFACE)

target_include_directories(algo_pack INTERFACE ${CMAKE_SOURCE_DIR}/include)


add_subdirectory(tests)
