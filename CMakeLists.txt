cmake_minimum_required(VERSION 3.15)
project(CLionProjects C)

set(CMAKE_C_STANDARD 11)

include_directories(Lab01)
include_directories(Lab04)

add_executable(CLionProjects
        Lab01/main.c
        Lab01/NFAFragmentStack.c
        Lab01/NFAFragmentStack.h
        Lab01/OutputResult.c
        Lab01/OutputResult.h
        Lab01/RegexpToNFA.h
        Lab01/RegexpToPost.c
        Lab01/RegexpToPost.h
        Lab04/main.c
        Lab04/RemoveLeftRecursion.h)
