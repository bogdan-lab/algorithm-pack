cmake_minimum_required(VERSION 3.19)

project(AlgorithmPack LANGUAGES CXX)

string(COMPARE EQUAL ${CMAKE_SOURCE_DIR} ${PROJECT_SOURCE_DIR} PROJECT_IS_TOP_LEVEL)

# Use old loaded module with warning flags from here
# https://github.com/lefticus/cppbestpractices/blob/master/02-Use_the_Tools_Available.md
include(cmake/CompilerWarnings.cmake)
add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings)

# Warnings are set as interface library do not set it here in order to prevent
# warning flag propagation to user targets
add_library(algo_pack INTERFACE)

target_include_directories(algo_pack INTERFACE ${CMAKE_SOURCE_DIR}/include)
target_compile_features(algo_pack INTERFACE cxx_std_17)

option(ENABLE_TESTING "Enables test build for the project." ${PROJECT_IS_TOP_LEVEL})
if(ENABLE_TESTING)
  message(STATUS "Building tests")
  enable_testing()
  add_subdirectory(tests)
endif()
