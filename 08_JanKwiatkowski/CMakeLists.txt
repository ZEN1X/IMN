cmake_minimum_required(VERSION 3.12)
project(lab7 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

include_directories(include src)
file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} src/main.cpp)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
