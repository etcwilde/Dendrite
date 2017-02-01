cmake_minimum_required(VERSION 2.6)
project(Dendrite)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#### Dependencies ####

find_package(Threads)

#### Compiler Support ####

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  set(CMAKE_COMPILER_IS_CLANGCC 1)
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
  set(CMAKE_COMPILER_IS_GNUCC 1)
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
  set(CMAKE_COMPILER_IS_ICC 1)
endif()

message(STATUS ${CMAKE_CXX_COMPILER_ID})

if (CMAKE_COMPILER_IS_CLANGCC OR CMAKE_COMPILER_IS_ICC)
  list (APPEND CXX_COMPILER_FLAGS -std=c++1z)
elseif (CMAKE_COMPILER_IS_ICC)
  message(WARNING "Untested Compiler: ${CMAKE_CXX_COMPILER_ID}")
elseif(CMAKE_COMPILER_IS_GNUCC)
  list (APPEND CXX_COMPILER_FLAGS -std=c++1z)
else()
  message(WARNING "Unknown Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
endif()

#### Components ####

add_subdirectory("extern/")
add_subdirectory("inc/")
add_subdirectory("src/")


#### Build ####

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_COMPILER_FLAGS}")


include_directories("inc/" "extern/")

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_INCLUDE_LIST} ${PROJECT_SOURCE_LIST})
target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
