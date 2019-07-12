cmake_minimum_required(VERSION 3.5.2)
project(RISC_V)

set(CMAKE_CXX_STANDARD 14)

add_executable(code main.cpp parser.hpp register.hpp statement.hpp five-stage.hpp)
target_compile_options(code PRIVATE -O2)