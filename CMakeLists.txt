cmake_minimum_required(VERSION 2.6)

project(KITC_LORA)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_COMPILER g++)

include_directories(include)

add_subdirectory(src)
add_subdirectory(main)

#
# compile options
#
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



