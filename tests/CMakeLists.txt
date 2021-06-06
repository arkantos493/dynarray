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
)
add_executable(test_cases ${CATCH_INCLUDE_DIR}/catch_main.cpp ${TEST_SOURCES})
#target_compile_features(test_cases PUBLIC cxx_std_20)
target_include_directories(test_cases PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(test_cases Catch)

# add test for CTest
add_test(NAME test COMMAND test_cases) # --success)