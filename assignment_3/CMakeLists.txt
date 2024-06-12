cmake_minimum_required(VERSION 3.28)
project(assignment_3 C)

set(CMAKE_C_STANDARD 99)

add_executable(assignment_3 caesar.c
        main.c
        helper.c
        helper.h)
