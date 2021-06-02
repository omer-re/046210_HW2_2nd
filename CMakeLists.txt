cmake_minimum_required(VERSION 3.14)
project(046210_HW2_2nd C)

set(CMAKE_C_STANDARD 11)

add_executable(046210_HW2_2nd kernel/list_manager.c kernel/list_manager.h kernel/syscall_modifications.c)