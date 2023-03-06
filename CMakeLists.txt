cmake_minimum_required(VERSION 3.24)
project(EasySlither)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

link_libraries(EasyXa EasyXw XInput)

add_executable(EasySlither
        src/main.cpp
        src/Menu.cppm
        src/Screen.cppm
        src/World.cppm
        src/Utils/Drawable.cppm
        src/Utils/Frames.cppm
        extra/application.manifest)
