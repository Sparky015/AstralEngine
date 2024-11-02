

add_library(ChessEngine
        Core/CoreMacroDefinitions.h
        Debug/DebugMacros.h
        Debug/DebugMacros.cpp
        pch.h
        pch.cpp
        Core/Events/EventBus.h
        Core/Events/EventListener.h
        Core/Events/EventPublisher.h
        Game/Board.cpp
        Game/Board.h
        Game/PieceInfo.h
        Game/PieceInfo.cpp
        IO/PrintBoardEvent.h
        Core/Events/Event.h
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
        Game/BoardManager.cpp
        Game/BoardManager.h
)


add_executable(Sozin
        main.cpp
)

target_link_libraries(Sozin PRIVATE ChessEngine)

# Link directories for Cmake (Point to where the lib files are for GLFW)
if (APPLE)
    target_link_directories(ChessEngine PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/mac-lib)
elseif (WIN32)
    target_link_directories(ChessEngine PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/windows-lib)
endif ()


if (APPLE)
    target_link_libraries(ChessEngine PRIVATE glfw.3)
elseif (WIN32)
    target_link_libraries(ChessEngine PRIVATE glfw3)
endif ()
target_link_libraries(ChessEngine PRIVATE ImGui)
target_link_libraries(ChessEngine PRIVATE GLAD)

target_include_directories(ChessEngine PUBLIC "${CMAKE_SOURCE_DIR}/Source")
target_precompile_headers(ChessEngine PRIVATE "${CMAKE_SOURCE_DIR}/Source/pch.h")

target_include_directories(ChessEngine PRIVATE "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/include")
target_include_directories(ChessEngine PRIVATE "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLAD")

target_compile_definitions(ChessEngine PRIVATE -DGLFW_INCLUDE_NONE)
target_compile_definitions(ChessEngine PRIVATE -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM)
