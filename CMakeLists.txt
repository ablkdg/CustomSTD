cmake_minimum_required(VERSION 3.26)
project(CustomSTD)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES main.cpp
        Custom/vector.h
        Custom/hash_map.h)

add_executable(CustomSTD ${SOURCE_FILES})
