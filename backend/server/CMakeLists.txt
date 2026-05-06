cmake_minimum_required(VERSION 3.16)

include(FetchContent)
FetchContent_Declare(
    crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG master
)
FetchContent_MakeAvailable(crow)

find_package(Threads REQUIRED)

file(GLOB_RECURSE SERVER_SOURCES "src/*.cpp")
list(FILTER SERVER_SOURCES EXCLUDE REGEX ".*main\\.cpp$")
add_library(server_lib STATIC ${SERVER_SOURCES})

target_include_directories(server_lib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(server_lib 
    PUBLIC 
        db 
        Crow::Crow 
        Threads::Threads 
        crypt
)

add_executable(server src/server_main.cpp)

target_link_libraries(server
    PRIVATE
        server_lib
)
