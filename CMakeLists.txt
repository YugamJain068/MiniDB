cmake_minimum_required(VERSION 3.16)

project(MiniDB)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    MiniDB
    src/main.cpp
    src/database/database.cpp
    src/storage/table.cpp
    src/storage/storage_manager.cpp
    src/parser/sql_parser.cpp
    src/parser/token.cpp
    src/parser/lexer.cpp
    src/executor/executor.cpp
)