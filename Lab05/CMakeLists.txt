cmake_minimum_required(VERSION 3.15)
project(Lab05 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Lab05
        main.c
        RemoveLeftRecursion.h)
