cmake_minimum_required(VERSION 3.12)
project(lab9 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find GSL (GNU Scientific Library)
find_package(GSL REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/lib")

add_executable(${PROJECT_NAME})

target_sources(
    ${PROJECT_NAME}
    PRIVATE src/main.cpp src/crank_nicolson.cpp src/crank_nicolson.h
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE $<$<CONFIG:Debug>:-Wall -Wextra -Wpedantic>
)

# Link the GSL libs
target_link_libraries(${PROJECT_NAME}
    PRIVATE GSL::gsl GSL::gslcblas
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
