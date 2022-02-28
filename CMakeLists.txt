cmake_minimum_required(VERSION 3.8)

set(This example) 

project(${This} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

add_subdirectory(googletest) 

set(Headers 
    Example.hpp
)

set(Sources 
    Example.cpp
)

add_library(${This} STATIC ${Sources} ${Headers})

add_subdirectory(test)