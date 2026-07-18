file(GLOB_RECURSE UEFORMAT_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(ueformat_test ${UEFORMAT_TEST_SOURCES})

target_include_directories(ueformat_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ueformat_test PRIVATE ueformat::ueformat ueformat::ueformat_c)

enable_testing()
add_test(NAME ueformat_test COMMAND ueformat_test)
