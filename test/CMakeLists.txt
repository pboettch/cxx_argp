cmake_minimum_required(VERSION 3.8)

add_executable(readme readme.cpp)
target_include_directories(readme PRIVATE ..)

add_executable(basic-test basic-test.cpp)
target_include_directories(basic-test PRIVATE ..)

enable_testing()

add_test(NAME basic-test
         COMMAND ./basic-test)
