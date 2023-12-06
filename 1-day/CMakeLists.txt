cmake_minimum_required(VERSION 3.5)

#PROJECT LIB
project(main)

set(CMAKE_C_STANDARD 23)

add_executable(main main.c)
#target_compile_options(main PRIVATE -Wall -Wextra -Werror -pedantic)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
