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
        src/Game/GameConfig.cppm
        src/Game/GameInterface.cppm
        src/Game/GameType.cppm
        src/Game/Prey.cppm
        src/Game/Sector.cppm
        src/Game/Snake.cppm
        src/Game/World.cppm
        src/Internal/GameServer.cppm
        src/Internal/GameStore.cppm
        src/Network/Connection.cppm
        src/Network/ConnectionEnd.cppm
        src/Network/ConnectionType.cppm
        src/Utils/Angle.cppm
        src/Utils/BoundBox.cppm
        src/Utils/Filesystem.cppm
        src/Utils/Frames.cppm
        src/Utils/Mediator.cppm
        src/Utils/Names.cppm
        src/Utils/Observer.cppm
        src/Utils/Random.cppm
        src/Utils/Throttle.cppm
        src/Utils/Time.cppm)

set(SOURCE_FILES_CLIENT
        src/client.cpp
        src/Internal/ScreenMediator.cppm
        src/Internal/UserInput.cppm
        src/Menu/Menu.cppm
        src/Menu/MenuInterface.cppm
        src/Menu/MenuItem.cppm
        src/Screen/GameScreen.cppm
        src/Screen/GameOverScreen.cppm
        src/Screen/LoadingScreen.cppm
        src/Screen/MainScreen.cppm
        src/Screen/RankScreen.cppm
        src/Screen/ScaleFactor.cppm
        src/Screen/Screen.cppm
        src/Screen/ScreenInterface.cppm
        src/Screen/ScreenName.cppm
        src/Screen/UsernameInput.cppm
        src/ThirdParty.cppm
        src/Utils/Colors.cppm
        src/Utils/Drawable.cppm
        src/Utils/InputBox.cppm
        src/Utils/TextEncode.cppm)

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
    set_target_properties(EasySlither PROPERTIES COMPILE_DEFINITIONS EASYSLITHER_CLIENT=1)
endif ()

add_executable(EasySlitherServer
        ${SOURCE_FILES_SERVER}
        ${SOURCE_FILES_UNIVERSAL})
set_target_properties(EasySlitherServer PROPERTIES COMPILE_DEFINITIONS EASYSLITHER_SERVER=1)
