include_guard()

# Macro which defines CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
# Ideally variable has to be defined by the system automatically, but it
# is not happening. MAcros will use external python script and initialize paths
# according to default compiler settings.
# Note that macros holds dirty hack in order to ensure my local build!
macro(add_standard_include_paths)
  if((NOT CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES) 
      AND (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" 
            OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang"))
    message(STATUS "Standard include directories will be initialized according to default compiler values.")
    execute_process(COMMAND ${CMAKE_SOURCE_DIR}/cmake/get_default_compiler_paths ${CMAKE_CXX_COMPILER}
                RESULT_VARIABLE COMMAND_RETURN
                OUTPUT_VARIABLE CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
                ERROR_VARIABLE ERROR_MESSAGE)
    if(NOT COMMAND_RETURN EQUAL 0)
      message(STATUS "Cannot define default compiler include paths. Reason: ${ERROR_MESSAGE}")
    else()
      # Dirty hack in order to avoid including path which brakes my local build
      list(LENGTH CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES STANDARD_INCLUDE_LENGTH)
      math(EXPR LAST_INDEX "${STANDARD_INCLUDE_LENGTH}-1")
      list(REMOVE_AT CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${LAST_INDEX})
    endif()
  endif()
endmacro()

# Privately links sanitizers and their options to the given target
function(enable_asan_and_ubsan TARGET)
  set(CONNECTION_TYPE "PRIVATE")
  get_target_property(TARGET_TYPE ${TARGET} TYPE)
  if(${TARGET_TYPE} STREQUAL "INTERFACE_LIBRARY")
    set(CONNECTION_TYPE "INTERFACE")
  endif()
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(${TARGET} ${CONNECTION_TYPE} -fsanitize=address,undefined 
                                             -fno-sanitize-recover=all)
    target_link_options(${TARGET} ${CONNECTION_TYPE} -fsanitize=address,undefined)
  else()
    message(STATUS "Currently build system does not support sanitizer option "
             "for compiler ${CMAKE_CXX_COMPILER_ID}")
  endif()
endfunction()