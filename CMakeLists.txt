cmake_minimum_required(VERSION 3.24)
project(FishMan CXX)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

link_libraries(EasyXa EasyXw)

add_executable(OnlyAHomework main.cpp application.manifest)
