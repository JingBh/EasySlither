cmake_minimum_required(VERSION 3.24)
project(EasySlither)

set(CMAKE_CXX_STANDARD 20)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG=1)
endif ()

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

link_libraries(EasyXa EasyXw XInput)

add_executable(EasySlither
        src/main.cpp
        src/Game/GameInterface.cppm
        src/Game/World.cppm
        src/Menu/Menu.cppm
        src/Menu/MenuInterface.cppm
        src/Menu/MenuItem.cppm
        src/Middleware/ScreenMediator.cppm
        src/Middleware/UserInput.cppm
        src/Screen/ScreenInterface.cppm
        src/Screen/MainScreen.cppm
        src/Screen/ScaleFactor.cppm
        src/Screen/Screen.cppm
        src/Screen/ScreenName.cppm
        src/Screen/UsernameInput.cppm
        src/ThirdParty.cppm
        src/Utils/Colors.cppm
        src/Utils/Drawable.cppm
        src/Utils/Frames.cppm
        src/Utils/Mediator.cppm
        src/Utils/Observer.cppm
        src/Utils/TextEncode.cppm
        src/Utils/Throttle.cppm
        src/Utils/Time.cppm
        extra/application.manifest)
