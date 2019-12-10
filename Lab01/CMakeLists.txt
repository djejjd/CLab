cmake_minimum_required(VERSION 3.15)
project(Lab01 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Lab01
        main.c
        NFAFragmentStack.c
        NFAFragmentStack.h
        OutputResult.c
        OutputResult.h
        RegexpToNFA.h
        RegexpToPost.c
        RegexpToPost.h)
