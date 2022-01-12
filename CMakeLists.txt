cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME GDAntiAliasing)
project(${PROJECT_NAME} VERSION 1.0.0)

file(GLOB_RECURSE SOURCES 
    dllmain.cpp
    src/*.cpp
)

set(WIN32 ON)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_subdirectory(sub/MinHook)
target_link_libraries(${PROJECT_NAME} sub/MinHook/Release/minhook.x32)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    sub/cocos2dx
    sub/cocos2dx/include
    sub/cocos2dx/kazmath/include
    sub/cocos2dx/platform/third_party/win32/OGLES
    sub/cocos2dx/platform/win32
    sub/extensions
    sub/MinHook/include
    sub/gd/include
    sub/gd
)

target_link_libraries(
    ${PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/sub/cocos2dx/libcocos2d.lib
    ${CMAKE_SOURCE_DIR}/sub/extensions/libExtensions.lib
)