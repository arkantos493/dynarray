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
function(register_test cxx_standard)
    # create executable
    set(TEST_CASE_NAME "test_cases_cxx${cxx_standard}")
    add_executable(${TEST_CASE_NAME} ${CATCH_INCLUDE_DIR}/catch_main.cpp ${TEST_SOURCES})
    target_include_directories(${TEST_CASE_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
    # link against Catch
    target_link_libraries(${TEST_CASE_NAME} Catch)
    # set requested C++ standard
    set_property(TARGET ${TEST_CASE_NAME}
            PROPERTY CXX_STANDARD ${cxx_standard}
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF)

    # add test for CTest
    set(TEST_NAME "test_cxx${cxx_standard}")
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_CASE_NAME})
endfunction()


# set compiler language standard flag
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU"
        OR CMAKE_CXX_COMPILER_ID MATCHES "Clang"
        OR CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    set(COMPILER_STD_FLAG "-std=c++")
elseif (CMAKE_CXX_COMPILED_ID MATCHES "MSVC")
    set(COMPILED_STD_FLAG "/std:c++")
else ()
    message(FATAL_ERROR "Unrecognized Compiler!")
endif ()

include(CheckCXXCompilerFlag)
# register tests
register_test(11)
check_cxx_compiler_flag(${COMPILER_STD_FLAG}14 HAS_FLAG_STD_CXX14)
if (HAS_FLAG_STD_CXX14)
    register_test(14)
endif ()
check_cxx_compiler_flag(${COMPILER_STD_FLAG}17 HAS_FLAG_STD_CXX17)
if (HAS_FLAG_STD_CXX17)
    register_test(17)
endif ()
check_cxx_compiler_flag(${COMPILER_STD_FLAG}20 HAS_FLAG_STD_CXX20)
if (HAS_FLAG_STD_CXX20)
    register_test(20)
endif ()