cmake_minimum_required(VERSION 3.16)
project(RoomSchedClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

file(GLOB CLIENT_ROOT_SOURCES 
    "*.cpp" 
    "*.hpp"
)
file(GLOB_RECURSE CLIENT_SUB_SOURCES 
    "api/*.cpp" 
    "api/*.hpp" 
    "ui/*.cpp" 
    "ui/*.hpp" 
    "ui/**/*.ui"
)

set(CLIENT_SOURCES ${CLIENT_ROOT_SOURCES} ${CLIENT_SUB_SOURCES})

add_executable(RoomSchedClient ${CLIENT_SOURCES})

file(GLOB_RECURSE UI_HEADERS_PATHS LIST_DIRECTORIES true "ui/*")

set(UI_INCLUDE_DIRS "")
foreach(ITEM ${UI_HEADERS_PATHS})
    if(IS_DIRECTORY ${ITEM})
        list(APPEND UI_INCLUDE_DIRS ${ITEM})
    endif()
endforeach()

target_include_directories(RoomSchedClient PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/api
    ${UI_INCLUDE_DIRS}
)

target_link_libraries(RoomSchedClient PRIVATE Qt6::Widgets Qt6::Network)
