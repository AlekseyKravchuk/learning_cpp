cmake_minimum_required(VERSION 3.28)
project(03_transport_guide_part_C_TO_SUBMIT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -pthread -O0")
set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
        03_transport_guide_part_C.cpp)


add_executable(${PROJECT_NAME} ${SOURCES})
