cmake_minimum_required(VERSION 3.9)
project(segment_tree)

add_library(segment_tree INTERFACE)

target_compile_features(segment_tree INTERFACE
    cxx_std_11
)

target_include_directories(segment_tree INTERFACE
    "${PROJECT_SOURCE_DIR}/include/"
)

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(googletest)

    add_executable(tests
        tests/segment_tree.cpp
    )

    target_compile_features(tests PRIVATE
        cxx_std_17
    )

    target_link_libraries(tests
        gtest
        gtest_main
        segment_tree
    )

    gtest_discover_tests(tests)
endif()