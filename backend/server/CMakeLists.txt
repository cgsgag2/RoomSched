cmake_minimum_required(VERSION 3.16)

add_compile_options(-DBOOST_ASIO_ENABLE_OLD_SERVICES)

include(FetchContent)

FetchContent_Declare(
    crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG master
)
FetchContent_MakeAvailable(crow)
find_package(Threads REQUIRED)

add_compile_definitions(BOOST_ASIO_ENABLE_OLD_SERVICES)

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
        Threads::Threads
        ws2_32
        mswsock
)
