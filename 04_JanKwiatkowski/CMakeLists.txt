cmake_minimum_required(VERSION 3.12)
project(lab4)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_executable(${PROJECT_NAME} main.cpp)

# macOS
if(APPLE)
    # Use `execute_process` to get the output of `brew --prefix libomp`
    execute_process(
        COMMAND brew --prefix libomp
        OUTPUT_VARIABLE BREW_LIBOMP_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(OpenMP_ROOT "${BREW_LIBOMP_PREFIX}")

    message(STATUS "Detected macOS: Setting OpenMP_ROOT to ${OpenMP_ROOT}")
endif()

# OpenMP
find_package(OpenMP REQUIRED)

if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
