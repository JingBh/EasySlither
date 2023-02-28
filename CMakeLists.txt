cmake_minimum_required(VERSION 3.24)
project(Assignment CXX)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

link_libraries(EasyXa EasyXw XInput)

add_executable(Assignment main.cpp exercise/1.cpp exercise/2.cpp exercise/3.cpp exercise/4.cpp src/utils.cpp application.manifest src/shapes.cpp src/shapes.h)
