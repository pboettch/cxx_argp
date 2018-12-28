cmake_minimum_required(VERSION 3.8)

find_package(Threads REQUIRED)

add_executable(cxx-argp-readme readme.cpp)
target_include_directories(cxx-argp-readme PRIVATE ..)

add_executable(basic-test basic-test.cpp)
target_include_directories(basic-test PRIVATE ..)

add_executable(app app.cpp)
target_include_directories(app PRIVATE ..)
target_link_libraries(app Threads::Threads)

enable_testing()

add_test(NAME basic-test
         COMMAND ./basic-test)

add_test(NAME app-without-args
         COMMAND app)

add_test(NAME app-with-right-args
         COMMAND app -h google.org)

set_tests_properties(
    app-with-right-args
        PROPERTIES
            TIMEOUT 3)

add_test(NAME app-with-wrong-args
         COMMAND app -h google.com)

set_tests_properties(
    app-without-args
    app-with-wrong-args
        PROPERTIES
            WILL_FAIL ON)
