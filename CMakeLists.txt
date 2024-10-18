cmake_minimum_required(VERSION 3.28)
project(Chess_Engine)

set(CMAKE_CXX_STANDARD 17)

add_executable(Chess_Engine main.cpp
        Source/Core/CoreMacroDefinitions.h
        Source/Core/Debug/DebugMacros.h
        Source/Core/Debug/DebugMacros.cpp
        Source/pch.h
        Source/pch.cpp
        "Source/Core/Events/EventBus.h"
        "Source/Core/Events/EventListener.h"
        "Source/Core/Events/EventPublisher.h"
)

target_include_directories(Chess_Engine PRIVATE "${CMAKE_SOURCE_DIR}/Source")
target_precompile_headers(Chess_Engine PRIVATE "${CMAKE_SOURCE_DIR}/Source/pch.h")