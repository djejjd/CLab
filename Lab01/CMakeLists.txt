cmake_minimum_required(VERSION 3.15)
project(Lab01)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Lab01
        input1.txt
        input2.txt
        input3.txt
        input4.txt
        input5.txt
        input6.txt
        input7.txt
        input8.txt
        main.c
        makefile
        NFAFragmentStack.c
        NFAFragmentStack.h
        output1.txt
        output2.txt
        output3.txt
        output4.txt
        output5.txt
        output6.txt
        output7.txt
        output8.txt
        OutputResult.c
        OutputResult.h
        RegexpToNFA.cplproj
        RegexpToNFA.h
        RegexpToNFA.puo
        RegexpToPost.c
        RegexpToPost.h
        sonar-project.properties)
