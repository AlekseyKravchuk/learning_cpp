cmake_minimum_required(VERSION 3.28.3)
project(21_task_pipeline)

set(SMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -pthread -O0")
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)

set(SOURCES
        pipeline.cpp)

set(HEADERS
        profile.h test_runner.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})