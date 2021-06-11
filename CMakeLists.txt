cmake_minimum_required(VERSION 3.20)
project(dynarray
        VERSION 1.0.0
        DESCRIPTION "Implementation of a runtime fixed-size array."
        LANGUAGES CXX)

add_executable(dynarray examples.cpp dynarray.hpp)

# minimum required C++ version is C++11
target_compile_features(dynarray PRIVATE cxx_std_11)

# enable testing
option(ENABLE_TESTS "Enable testing using Catch2" OFF)
if (ENABLE_TESTS)
    message(STATUS "Enabled testing using Catch2.")
    enable_testing()
    add_subdirectory(tests)
endif ()