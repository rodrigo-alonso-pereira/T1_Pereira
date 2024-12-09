cmake_minimum_required(VERSION 3.30)
project(T1_Pereira C)

set(CMAKE_C_STANDARD 11)

add_executable(ProcesamientoCarga T1_Pereira.c)
add_executable(JohnsonTrotter_algorithm JohnsonTrotter_algorithm.c)
add_executable(test test.c)
