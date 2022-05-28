cmake_minimum_required(VERSION 3.19)

project(AlgorithmPack LANGUAGES CXX)

add_library(algo_pack INTERFACE)

target_include_directories(algo_pack INTERFACE ${CMAKE_SOURCE_DIR}/include)
target_compile_features(algo_pack INTERFACE cxx_std_17)

add_subdirectory(tests)
