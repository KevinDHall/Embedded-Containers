cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("embedded_containers_test")

message ("${CMAKE_CXX_COMPILER}")       # C:/dev/MinGW/bin/g++.exe
message ("${CMAKE_CXX_COMPILER_ID}")    # EMPTY
message ("${CMAKE_COMPILER_IS_GNUCXX}") # EMPTY

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
	set(cppstandard "-std=c++14")
	set(cstandard "-std=c11")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
	set(cppstandard "")
	set(cstandard "")
endif()
if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${cppstandard} ${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS   "${cstandard} ${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../catch/include ${CMAKE_CURRENT_SOURCE_DIR}/../../src)

add_executable(test_runner test_runner.cpp)

add_test(test test_runner)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")
