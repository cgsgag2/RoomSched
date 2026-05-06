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

FetchContent_MakeAvailable(crow jwt-cpp)

find_package(Threads REQUIRED)

file(GLOB_RECURSE SERVER_SOURCES "src/*.cpp")
add_executable(server ${SERVER_SOURCES})

target_include_directories(server
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(server
    PRIVATE
        db
        Crow::Crow
        jwt-cpp::jwt-cpp
        Threads::Threads
        crypt
)
