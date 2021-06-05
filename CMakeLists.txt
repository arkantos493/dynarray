cmake_minimum_required(VERSION 3.20)
project(dynarray
        VERSION 1.0.0
        DESCRIPTION "Implementation of a runtime fixed-size array."
        LANGUAGES CXX)

add_executable(dynarray examples.cpp dynarray.hpp)

target_compile_features(dynarray PUBLIC cxx_std_20)
