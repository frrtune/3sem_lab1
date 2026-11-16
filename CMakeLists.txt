cmake_minimum_required(VERSION 3.23)
project(lab1 CXX)
 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
 
enable_testing()
 
add_subdirectory(googletest-1.17.0)
add_subdirectory(binary_tree)
add_subdirectory(types)
add_subdirectory(tests)

add_executable(lab1_ui main.cpp)