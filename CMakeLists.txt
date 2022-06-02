cmake_minimum_required(VERSION 3.19)

project(AlgorithmPack LANGUAGES CXX)

string(COMPARE EQUAL ${CMAKE_SOURCE_DIR} ${PROJECT_SOURCE_DIR} PROJECT_IS_TOP_LEVEL)

add_library(algo_pack INTERFACE)

target_include_directories(algo_pack INTERFACE ${CMAKE_SOURCE_DIR}/include)


option(ENABLE_TESTING "Enables test build for the project." ${PROJECT_IS_TOP_LEVEL})
if(ENABLE_TESTING)
  message(STATUS "Building tests")
  enable_testing()
  add_subdirectory(tests)
endif()
