cmake_minimum_required(VERSION 3.26)
project(hashtable C)

set(CMAKE_C_STANDARD 99)

add_executable(hashtable main.c
        hashtable.c)
