cmake_minimum_required(VERSION 3.16)
project(LibraryManagementSystem LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(library_management
    src/main.cpp
    src/Book.cpp
    src/Member.cpp
    src/Library.cpp
)
target_include_directories(library_management PRIVATE include)

if(MSVC)
    target_compile_options(library_management PRIVATE /W4)
else()
    target_compile_options(library_management PRIVATE -Wall -Wextra -Wpedantic)
endif()
