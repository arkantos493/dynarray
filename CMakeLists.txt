cmake_minimum_required(VERSION 3.19)
project(dynarray
        VERSION 1.0.0
        DESCRIPTION "Implementation of a runtime fixed-size array."
        LANGUAGES CXX)

add_executable(dynarray examples.cpp dynarray.hpp)

target_compile_features(dynarray PUBLIC cxx_std_20)


if (MSVC)
    # warning level 4 and all warnings as errors
    target_compile_options(dynarray PUBLIC /W4 /WX)
else ()
    # lots of warnings and all warnings as errors
    target_compile_options(dynarray PUBLIC -Wall -Wextra -pedantic -Werror)
endif ()