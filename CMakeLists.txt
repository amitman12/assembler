cmake_minimum_required(VERSION 3.4.1)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

project(assembler C)


if (CMAKE_BUILD_TYPE AND
    NOT CMAKE_BUILD_TYPE MATCHES "^(Debug|Release|RelWithDebInfo|MinSizeRel)$")
  message(FATAL_ERROR "Invalid value for CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()

if (CMAKE_BUILD_TYPE MATCHES "^(Debug)$")
  set (CMAKE_C_FLAGS "-DDEBUG ${CMAKE_C_FLAGS}")
endif()

####################################################################################################
# lib
####################################################################################################

# add src files
file(GLOB GLOB_RESULT "./*.c")
list(APPEND C_FILES ${GLOB_RESULT})

file(GLOB GLOB_RESULT "./*.h")
list(APPEND H_FILES ${GLOB_RESULT})

add_executable(main ${C_FILES})
