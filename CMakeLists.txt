cmake_minimum_required(VERSION 3.10)

project(
    ttwinstaller-qt6
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

#file(GLOB_RECURSE INSTALLER "src/Installer/*.cpp")
#add_executable(installer ${INSTALLER})
add_executable(installer
    src/Installer/Main.cpp
    src/Installer/Window.cpp
    src/Installer/InputSection.cpp)
target_link_libraries(installer PRIVATE Qt6::Widgets)

#file(GLOB_RECURSE PATCHMAKER "src/PatchMaker/*.cpp")
#add_executable(patchmaker ${PATCHMAKER})
add_executable(patchmaker
    src/PatchMaker/Main.cpp
    src/PatchMaker/Window.cpp)
target_link_libraries(patchmaker PRIVATE Qt6::Widgets)

