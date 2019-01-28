cmake_minimum_required(VERSION 3.2)

find_package(Threads REQUIRED)

if(NOT TARGET cxx-argp)
    add_library(cxx-argp INTERFACE)
    target_include_directories(cxx-argp
        INTERFACE
            ..)
    target_compile_features(cxx-argp
        INTERFACE
            cxx_range_for) # for C++11 - flags
endif()

add_executable(cxx-argp-readme readme.cpp)
target_link_libraries(cxx-argp-readme PRIVATE cxx-argp)

add_executable(basic-test basic-test.cpp)
target_link_libraries(basic-test PRIVATE cxx-argp)

add_executable(app app.cpp)
target_link_libraries(app PRIVATE cxx-argp Threads::Threads)

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
