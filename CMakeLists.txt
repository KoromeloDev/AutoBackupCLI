cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(AutoBackup VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_AUTOMOC true)

add_subdirectory(import)
add_subdirectory(src)
