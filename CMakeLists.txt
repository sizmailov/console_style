cmake_minimum_required(VERSION 3.1.3)
project(console_style)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES ConsoleSyle.cpp)
add_library(console_style ${SOURCE_FILES})

add_executable(console_style_example console_style_example.cpp)
target_link_libraries(console_style_example console_style)
