cmake_minimum_required(VERSION 3.12)
project(lab5)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_executable(${PROJECT_NAME} main.cpp)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
