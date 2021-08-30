cmake_minimum_required(VERSION 3.20)
project (API21 C)

set(CMAKE_C_STANDARD 11)
#set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -g")
set(GCC_COVERAGE_COMPILE_FLAGS "-DEVAL -Wall -Werror -std=gnu11 -O2 -pipe -static -s -o -lm")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
add_executable(EseguibileAPI api21.c)
