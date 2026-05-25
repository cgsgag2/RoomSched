FetchContent_MakeAvailable(crow jwt-cpp)
cmake_minimum_required(VERSION 3.16)

include(FetchContent)
FetchContent_Declare(
    crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG master
)
FetchContent_Declare(
    jwt-cpp
    GIT_REPOSITORY https://github.com/Thalhammer/jwt-cpp.git
    GIT_TAG v0.7.0
)
    jwt-cpp
    GIT_REPOSITORY https://github.com/Thalhammer/jwt-cpp.git
    GIT_TAG v0.7.0
)

FetchContent_MakeAvailable(crow jwt-cpp)

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
        jwt-cpp::jwt-cpp
)

add_executable(server src/server_main.cpp)

target_link_libraries(server
    PRIVATE
        server_lib
)
