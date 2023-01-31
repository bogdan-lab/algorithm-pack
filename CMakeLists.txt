cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 17)

# Add project_options v0.26.3
# https://github.com/aminya/project_options
# Change the version in the following URL to update the package (watch the releases of the repository for future updates)
include(FetchContent)
FetchContent_Declare(_project_options URL https://github.com/aminya/project_options/archive/refs/tags/v0.25.2.zip)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)
message(STATUS "ProjectOptions directory = ${_project_options_SOURCE_DIR}")

project(AlgorithmPack LANGUAGES CXX)

# List which will accumulate all project options used during development
set(ALPA_DEV_OPTOINS "")

option(ALPA_STATIC_ANALYSIS "Enables static analisys tools (cppcheck and clang-tidy)" ON)

if(ALPA_STATIC_ANALYSIS)
  list(APPEND ALPA_DEV_OPTIONS 
                    ENABLE_CPPCHECK
                    CPPCHECK_OPTIONS 
                        # --template is set automatically inside project_options internals according to the generator
                        --enable=all 
                        --inconclusive 
                        --language=c++
                        # --std is set automatically inside project_options internals according to CMAKE_CXX_STANDARD value
                        --cppcheck-build-dir="${CMAKE_CURRENT_BINARY_DIR}/cppcheck"
                        --inline-suppr
                    ENABLE_CLANG_TIDY)
endif()



# Initialize project_options variable related to this project
# This overwrites `project_options` and sets `project_warnings`
# uncomment to enable the options. Some of them accept one or more inputs:
project_options(
      #${ALPA_DEV_OPTIONS}
      # ENABLE_DOXYGEN
      # ENABLE_SANITIZER_ADDRESS
      # ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
      # ENABLE_SANITIZER_MEMORY
      # WARNINGS_AS_ERRORS
      ENABLE_INCLUDE_WHAT_YOU_USE
)


include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Utils.cmake)
add_standard_include_pathes()


# Warnings are set as interface library do not set it here in order to prevent
# warning flag propagation to user targets
add_library(algo_pack INTERFACE)

target_include_directories(algo_pack INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

option(ALPA_ENABLE_TESTING "Enables test build for the project." ON)
if(ALPA_ENABLE_TESTING)
  message(STATUS "Building tests")
  enable_testing()
  add_subdirectory(tests)
endif()
