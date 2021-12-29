cmake_minimum_required(VERSION 3.20)
project(BigInt C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(BigInt
        number.h
        EXAMPLE.c)
