include_guard()

# Macro which defines CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
# Ideally variable has to be defined by the system automatically, but it
# is not happening. MAcros will use external python script and initialize pathes
# according to default compiler settings.
# Note that macros holds dirty hack in order to ensure my local build!
macro(add_standard_include_pathes)
  if((NOT CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES) 
      AND (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" 
            OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang"))
    message(STATUS "Standard include directories will be initialized according to default compiler values.")
    execute_process(COMMAND ${CMAKE_SOURCE_DIR}/cmake/get_default_compiler_pathes ${CMAKE_CXX_COMPILER}
                RESULT_VARIABLE COMMAND_RETURN
                OUTPUT_VARIABLE CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
                ERROR_VARIABLE ERROR_MESSAGE)
    if(NOT COMMAND_RETURN EQUAL 0)
      message(STATUS "Cannot define default compiler include pathes. Reasone: ${ERROR_MESSAGE}")
    else()
      # Dirty hack in order to avoid including path which brakes my local build
      list(LENGTH CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES STANDARD_INCLUDE_LENGTH)
      math(EXPR LAST_INDEX "${STANDARD_INCLUDE_LENGTH}-1")
      list(REMOVE_AT CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${LAST_INDEX})
    endif()
  endif()
endmacro()

# Privately links sanitizeers and their options to the given target
function(enable_asan_and_ubsan TARGET)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    list(APPEND SANITIZERS "address,undefined")
    target_compile_options(${TARGET} PRIVATE -fsanitize=address,undefined 
                                             -fno-sanitize-recover=all)
    target_link_options(${TARGET} PRIVATE -fsanitize=address,undefined)
  else()
    message(STATUS "Currently build system does not support sanitizer option "
             "for compiler ${CMAKE_CXX_COMPILER_ID}")
  endif()
endfunction()