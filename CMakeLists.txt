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

set(ALPA_CPPCHECK_OPTIONS
      # --template is set automatically inside project_options internals according to the generator
      --enable=all 
      --inconclusive 
      --language=c++
      # --std is set automatically inside project_options internals according to CMAKE_CXX_STANDARD value
      --cppcheck-build-dir="${CMAKE_BINARY_DIR}/cppcheck"
      --inline-suppr
)


# Initialize project_options variable related to this project
# This overwrites `project_options` and sets `project_warnings`
# uncomment to enable the options. Some of them accept one or more inputs:
project_options(
      # ENABLE_CACHE
       ENABLE_CPPCHECK
       CPPCHECK_OPTIONS ${ALPA_CPPCHECK_OPTIONS}
      # ENABLE_CLANG_TIDY
      # ENABLE_VS_ANALYSIS
      # ENABLE_CONAN
      # ENABLE_INTERPROCEDURAL_OPTIMIZATION
      # ENABLE_NATIVE_OPTIMIZATION
      # ENABLE_DOXYGEN
      # ENABLE_COVERAGE
      # ENABLE_SANITIZER_ADDRESS
      # ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
        #ENABLE_SANITIZER_THREAD
      #  ENABLE_SANITIZER_MEMORY
      # ENABLE_PCH
      # PCH_HEADERS
      # WARNINGS_AS_ERRORS
      # ENABLE_INCLUDE_WHAT_YOU_USE
      # ENABLE_BUILD_WITH_TIME_TRACE
      # ENABLE_UNITY
      # LINKER "lld"
      # CONAN_PROFILE ${profile_path}  # passes a profile to conan: see https://docs.conan.io/en/latest/reference/profiles.html
)

# Warnings are set as interface library do not set it here in order to prevent
# warning flag propagation to user targets
add_library(algo_pack INTERFACE)

target_include_directories(algo_pack INTERFACE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(algo_pack INTERFACE project_options)
target_compile_features(algo_pack INTERFACE cxx_std_17)

option(ENABLE_TESTING "Enables test build for the project." ON)
if(ENABLE_TESTING)
  message(STATUS "Building tests")
  enable_testing()
  add_subdirectory(tests)
endif()
