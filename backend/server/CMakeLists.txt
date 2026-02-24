cmake_minimum_required(VERSION 3.16)

include(FetchContent)

FetchContent_Declare(
    crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG v1.0+5
)
FetchContent_MakeAvailable(crow)
find_package(Threads REQUIRED)

file(GLOB SERVER_SOURCES "src/*.cpp")
add_executable(server ${SERVER_SOURCES})

target_include_directories(server
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(server
    PRIVATE
        db
        Crow::Crow
        Threads::Threads
)
