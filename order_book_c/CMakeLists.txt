cmake_minimum_required(VERSION 3.10.2)

project(orderbook)

add_compile_options(-Wall)

file(GLOB orderbook_SRC
    "src/*.cpp"
    "test/*.cpp"
)

add_executable("orderbook" ${orderbook_SRC} test/main.cpp)
include_directories(include data)