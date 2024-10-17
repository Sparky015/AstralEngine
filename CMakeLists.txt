cmake_minimum_required(VERSION 3.28)
project(Chess_Engine)

set(CMAKE_CXX_STANDARD 17)

add_executable(Chess_Engine main.cpp
        Source/CoreMacroDefinitions.h
        Source/Debug/DebugMacros.h
        Source/Debug/DebugMacros.cpp
        Source/pch.h
        Source/pch.cpp
)

target_include_directories(Chess_Engine PRIVATE "${CMAKE_SOURCE_DIR}/Source")
target_precompile_headers(Chess_Engine PRIVATE "${CMAKE_SOURCE_DIR}/Source/pch.h")