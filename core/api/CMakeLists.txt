cmake_minimum_required(VERSION 3.20)

project(ueformat_c LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE UEFORMAT_API_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/ueformat/*.cpp"
)

add_library(ueformat_c STATIC ${UEFORMAT_API_SOURCES})
add_library(ueformat::ueformat_c ALIAS ueformat_c)

target_include_directories(ueformat_c PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ueformat_c PUBLIC ueformat::ueformat)
