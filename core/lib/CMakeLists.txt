cmake_minimum_required(VERSION 3.20)

project(ueformat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE UEFORMAT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/ueformat/*.cpp"
)

add_library(ueformat STATIC ${UEFORMAT_SOURCES})
add_library(ueformat::ueformat ALIAS ueformat)

target_include_directories(ueformat PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ueformat PRIVATE ZLIB::ZLIBSTATIC libzstd_static)
