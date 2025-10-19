cmake_minimum_required(VERSION 3.10)

project(Cannon)

set(CMAKE_C_STANDARD 99)

find_package(OpenMP REQUIRED)

add_library(cannon src/cannon.c)
target_link_libraries(cannon OpenMP::OpenMP_C)
#target_compile_options(cannon PRIVATE -O0)

add_executable(main src/main.c)
target_link_libraries(main cannon)
target_link_libraries(main OpenMP::OpenMP_C)