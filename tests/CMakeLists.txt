# add catch library
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# specify test source files and build executable
set(TEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/types.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/construction.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/assignment.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/element_access.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/iterators.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/capacity.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/operations.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/non_member_functions.cpp)


# function to add test cases using different C++ standards
include(CheckCXXCompilerFlag)
function(register_test cxx_standard_flag)
    # if MSVC is used, change compiler flag
    if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        string(REPLACE "=" ":" cxx_standard_flag ${cxx_standard_flag})
    endif ()
    string(SUBSTRING ${cxx_standard_flag} 8 -1 CXX_STANDARD_VALUE)

    # check if compiler flag is supported
    check_cxx_compiler_flag(${cxx_standard_flag} HAS_FLAG_STD_CXX_${CXX_STANDARD_VALUE})
    if (${HAS_FLAG_STD_CXX_${CXX_STANDARD_VALUE}})
        # create executable
        string(SUBSTRING ${cxx_standard_flag} 8 -1 CXX_STANDARD_VALUE)
        set(TEST_CASE_NAME "test_cases_cxx${CXX_STANDARD_VALUE}")
        add_executable(${TEST_CASE_NAME} ${CATCH_INCLUDE_DIR}/catch_main.cpp ${TEST_SOURCES})
        target_include_directories(${TEST_CASE_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
        # link against Catch
        target_link_libraries(${TEST_CASE_NAME} Catch)
        # set requested C++ standard
        target_compile_options(${TEST_CASE_NAME} PUBLIC ${cxx_standard_flag})

        # add test for CTest
        set(TEST_NAME "test_cxx${CXX_STANDARD_VALUE}")
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_CASE_NAME})
    endif ()
    unset(${HAS_FLAG_STD_CXX_${CXX_STANDARD_VALUE}} CACHE)
endfunction()


# register tests
register_test(-std=c++11)
register_test(-std=c++14)
register_test(-std=c++17)
register_test(-std=c++20)
register_test(-std=c++23)