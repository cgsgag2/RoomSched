cmake_minimum_required(VERSION 3.16)

find_package(GTest REQUIRED)

file(GLOB_RECURSE TEST_SOURCES "unit/*.cpp" "integration/*.cpp")
add_executable(backend_tests ${TEST_SOURCES} main.cpp)

target_include_directories(backend_tests 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(backend_tests 
    PRIVATE
        GTest::GTest
        db
        server_lib
)
