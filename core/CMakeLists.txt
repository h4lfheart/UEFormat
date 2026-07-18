cmake_minimum_required(VERSION 3.20)

project(ueformat_core LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external)
add_subdirectory(lib)
add_subdirectory(api)
add_subdirectory(test)
