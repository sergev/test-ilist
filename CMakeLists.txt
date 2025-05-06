cmake_minimum_required(VERSION 3.10)
project(IListTest)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
enable_testing()

# Build tests
add_executable(ilist_test ilist_test.cc)
target_link_libraries(ilist_test GTest::gtest_main)
gtest_discover_tests(ilist_test EXTRA_ARGS --gtest_repeat=1 PROPERTIES TIMEOUT 10)
