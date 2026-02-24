cmake_minimum_required(VERSION 3.16)

find_package(PkgConfig REQUIRED)
# Check that libpqxx is installed
pkg_check_modules(PQXX REQUIRED libpqxx)

file(GLOB DB_SOURCES "src/*.cpp")

add_library(db STATIC ${DB_SOURCES})

target_include_directories(db
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${PQXX_INCLUDE_DIRS}
)

target_link_libraries(db
    PUBLIC
        ${PQXX_LIBRARIES}
)
