cmake_minimum_required(VERSION 3.1.3)
project(console_style)

set(CMAKE_CXX_STANDARD 11)


include_directories(include)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp )

add_library(${PROJECT_NAME} ${SOURCE_FILES})

add_executable(${PROJECT_NAME}_example example.cpp)
target_link_libraries(${PROJECT_NAME}_example ${PROJECT_NAME})

set(${PROJECT_NAME}_INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}/include
        CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
