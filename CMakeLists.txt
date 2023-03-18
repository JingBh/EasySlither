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

set(SOURCE_FILES_UNIVERSAL
        src/Game/GameInterface.cppm
        src/Game/World.cppm
        src/Utils/Frames.cppm
        src/Utils/Mediator.cppm
        src/Utils/Observer.cppm
        src/Utils/TextEncode.cppm
        src/Utils/Throttle.cppm
        src/Utils/Time.cppm)

set(SOURCE_FILES_CLIENT
        src/client.cpp
        src/Menu/Menu.cppm
        src/Menu/MenuInterface.cppm
        src/Menu/MenuItem.cppm
        src/Middleware/ScreenMediator.cppm
        src/Middleware/UserInput.cppm
        src/Screen/LoadingScreen.cppm
        src/Screen/MainScreen.cppm
        src/Screen/ScaleFactor.cppm
        src/Screen/Screen.cppm
        src/Screen/ScreenInterface.cppm
        src/Screen/ScreenName.cppm
        src/Screen/UsernameInput.cppm
        src/ThirdParty.cppm
        src/Utils/Colors.cppm
        src/Utils/Drawable.cppm)

set(SOURCE_FILES_SERVER
        src/server.cpp)

set(RESOURCE_FILES
        assets/app.rc
        assets/extra/application.manifest)

if (WIN32)
    set(CMAKE_RC_COMPILER_INIT windres)
    enable_language(RC)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
    add_executable(EasySlither WIN32
            ${SOURCE_FILES_CLIENT}
            ${SOURCE_FILES_UNIVERSAL}
            ${RESOURCE_FILES})
endif ()

add_executable(EasySlitherServer
        ${SOURCE_FILES_SERVER}
        ${SOURCE_FILES_UNIVERSAL})
