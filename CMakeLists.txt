cmake_minimum_required(VERSION 3.20)
project(dynarray
        VERSION 1.0.0
        DESCRIPTION "Implementation of a runtime fixed-size array."
        LANGUAGES CXX)

add_executable(dynarray examples.cpp dynarray.hpp)

# enable testing
option(ENABLE_TESTS "Enable testing using Catch2" OFF)
if (ENABLE_TESTS)
    message(STATUS "Enabled testing using Catch2.")
    enable_testing()
    add_subdirectory(tests)
endif ()