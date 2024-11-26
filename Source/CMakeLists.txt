

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_definitions(-DDEBUG_BUILD)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_definitions(-DRELEASE_BUILD)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    add_definitions(-DMINIMUM_SIZE_RELEASE_BUILD)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    add_definitions(-DRELEASE_WITH_DEBUG_INFO_BUILD)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Default")
    add_definitions(-DRELEASE_WITH_DEBUG_INFO_BUILD)
endif()


add_library(ChessEngine
        Core/CoreMacroDefinitions.h
        Debug/DebugMacros.h
        Debug/DebugMacros.cpp
        pch.h
        pch.cpp
        Core/Events/EventBus.h
        Core/Events/EventListener.h
        Core/Events/EventPublisher.h
        Game/Board/Board.cpp
        Game/Board/Board.h
        Game/PieceInfo.h
        Game/PieceInfo.cpp
        IO/PrintBoardEvent.h
        IO/IOManager.cpp
        IO/IOManager.h
        Core/Engine.h
        Core/Engine.cpp
        Core/SystemEvents.h
        Core/SystemManager.h
        Core/SystemManager.cpp
        Window/WindowManager.cpp
        Window/WindowManager.h
        Debug/AllocationTracker.h
        Window/WindowEvents.h
        Debug/DebugManager.h
        Debug/DebugManager.cpp
        Debug/ImGuiDependencies/imgui_impl_glfw.cpp
        Debug/ImGuiDependencies/imgui_impl_glfw.h
        Debug/ImGuiDependencies/imgui_impl_opengl3.cpp
        Debug/ImGuiDependencies/imgui_impl_opengl3.h
        Debug/DebugEvents.h
        Game/Board/BoardManager.cpp
        Game/Board/BoardManager.h
        Debug/EngineDebugInfoLogger.h
        Debug/EngineDebugInfoLogger.cpp
        Debug/ConsoleLogFile.cpp
        Debug/ConsoleLogFile.h
        Debug/ProfilerLogFile.cpp
        Debug/ProfilerLogFile.h
        Game/Board/InternalBoardData.cpp
        Game/Pieces/Bishop.h
        Game/Pieces/Bishop.cpp
        Game/Pieces/ChessPiece.cpp
        Game/Pieces/King.h
        Game/Pieces/King.cpp
        Game/Pieces/Knight.cpp
        Game/Pieces/Pawn.cpp
        Game/Pieces/Queen.cpp
        Game/Pieces/Rook.cpp
        Game/MoveList.cpp
        Game/Board/Conversions.h
        Game/Board/Conversions.cpp
        Game/MoveGenerationHelpers.cpp
        Game/MoveGenerationHelpers.h
)


add_executable(Sozin main.cpp)

target_link_libraries(Sozin PRIVATE ChessEngine)
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sozin)

# Link directories for Cmake (Point to where the lib files are for GLFW)
if (APPLE)
    target_link_directories(ChessEngine PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/mac-lib)
elseif (WIN32)
    target_link_directories(ChessEngine PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/windows-lib)
endif ()

target_link_libraries(ChessEngine PRIVATE ImGui)
target_link_libraries(ChessEngine PRIVATE GLAD)

if (APPLE)
    target_link_libraries(ChessEngine PRIVATE glfw.3)
elseif (WIN32)
    target_link_libraries(ChessEngine PRIVATE glfw3)
endif ()


target_include_directories(ChessEngine PUBLIC "${CMAKE_SOURCE_DIR}/Source")
target_precompile_headers(ChessEngine PRIVATE "${CMAKE_SOURCE_DIR}/Source/pch.h")

target_include_directories(ChessEngine PRIVATE "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/include")
target_include_directories(ChessEngine PRIVATE "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLAD")


# Defining Macros

target_compile_definitions(ChessEngine PRIVATE -DGLFW_INCLUDE_NONE)
target_compile_definitions(ChessEngine PRIVATE -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM)
if (APPLE)
    target_compile_definitions(ChessEngine PRIVATE -DPLATFORM_MACOS)
elseif (WIN32)
    target_compile_definitions(ChessEngine PRIVATE -DPLATFORM_WINDOWS)
endif ()
target_compile_definitions(ChessEngine PRIVATE -DTURN_ON_LOGGING_CONSOLE_TO_FILE)
target_compile_definitions(ChessEngine PRIVATE -DROOT_DIR="${CMAKE_SOURCE_DIR}")
