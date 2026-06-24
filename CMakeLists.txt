cmake_minimum_required(VERSION 3.16)

project(MiniDB)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    MiniDB
    src/main.cpp
    src/database.cpp
    src/table.cpp
)