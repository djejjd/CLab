cmake_minimum_required(VERSION 3.15)
project(Lab04 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Lab04
        main.c
        RemoveLeftRecursion.h)
