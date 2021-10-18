cmake_minimum_required(VERSION 3.8)

project(febo-analyzer)

set(CMAKE_CXX_FLAGS "-Wall -Wno-sign-compare")

set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDES})
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)

add_library(febo-analyzer ${SRC_FILES})
target_compile_features(febo-analyzer PUBLIC cxx_std_20)
target_compile_features(febo-analyzer PUBLIC c_std_11)

find_package (Threads)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
enable_testing()


##############
# Unit Tests
##############
add_executable(runUnitTests ${TEST_SRC_FILES})
target_link_libraries(runUnitTests gtest gtest_main Threads::Threads)
target_link_libraries(runUnitTests febo-analyzer Threads::Threads)
add_test(UnitTests runUnitTests)
