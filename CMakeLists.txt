cmake_minimum_required(VERSION 3.19)

project(AlgorithmPack LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

################################
### Loading required modules ###
################################
# Add project_options v0.26.3
# https://github.com/aminya/project_options
# Change the version in the following URL to update the package (watch the releases of the repository for future updates)
include(FetchContent)
FetchContent_Declare(_project_options URL https://github.com/aminya/project_options/archive/refs/tags/v0.25.2.zip)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)
message(STATUS "ProjectOptions directory = ${_project_options_SOURCE_DIR}")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Utils.cmake)

######################
### Library itself ###
######################
add_library(algo_pack INTERFACE)
target_include_directories(algo_pack INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

################################
### Documentation definition ###
################################
option(ALPA_ENABLE_DOXYGEN_DOCS "Enables creation of doxygen-docs target in the build" ON)
if(ALPA_ENABLE_DOXYGEN_DOCS)
  set(DOXYGEN_EXCLUDE_PATTERNS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*" "${CMAKE_CURRENT_BINARY_DIR}/*")
  # macro is defined in https://github.com/aminya/project_options internals
  enable_doxygen("awesome-sidebar")
endif()

#####################################
### Test environment subdirectory ###
#####################################
option(ALPA_ENABLE_TESTING "Enables test build for the project." ON)
if(ALPA_ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
