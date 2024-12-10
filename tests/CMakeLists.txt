cmake_minimum_required(VERSION 3.10)
project(hello_test)

enable_testing()

add_subdirectory(googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(hello_test hello_test.cpp)
target_link_libraries(hello_test gtest gtest_main)

add_test(NAME hello_test COMMAND hello_test)