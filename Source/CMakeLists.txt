

set(SOURCE_FILES
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
        IOManager/PrintBoardEvent.h
        Core/Events/Event.h
        IOManager/IOManager.cpp
        IOManager/IOManager.h
        Core/Engine.h
        Core/Engine.cpp
        Core/SystemEvents.h
        Core/SystemManager.h
        Core/SystemManager.cpp
        WindowManager/WindowManager.cpp
        WindowManager/WindowManager.h
        Debug/AllocationTracker.h
        WindowManager/WindowEvents.h
        Debug/DebugManager.h
        Debug/DebugManager.cpp
        Debug/ImGuiDependencies/imgui_impl_glfw.cpp
        Debug/ImGuiDependencies/imgui_impl_glfw.h
        Debug/ImGuiDependencies/imgui_impl_opengl3.cpp
        Debug/ImGuiDependencies/imgui_impl_opengl3.h


)

set(SOURCE_FILES ${SOURCE_FILES} CACHE INTERNAL "Source Files")


add_executable(Chess_Engine
        main.cpp
        ${SOURCE_FILES}
)


# Link directories for Cmake (Point to where the lib files are for GLFW)
if (APPLE)
    target_link_directories(Chess_Engine PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/mac-lib)
elseif (WIN32)
    target_link_directories(Chess_Engine PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/windows-lib)
endif ()


if (APPLE)
    target_link_libraries(Chess_Engine PRIVATE glfw.3)
elseif (WIN32)
    target_link_libraries(Chess_Engine PRIVATE glfw3)
endif ()
target_link_libraries(Chess_Engine PRIVATE ImGui)
target_link_libraries(Chess_Engine PRIVATE GLAD)

target_include_directories(Chess_Engine PRIVATE "${CMAKE_SOURCE_DIR}/Source")
target_precompile_headers(Chess_Engine PRIVATE "${CMAKE_SOURCE_DIR}/Source/pch.h")

target_include_directories(Chess_Engine PRIVATE "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/include")
target_include_directories(Chess_Engine PRIVATE "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLAD")

target_compile_definitions(Chess_Engine PRIVATE -DGLFW_INCLUDE_NONE)
target_compile_definitions(Chess_Engine PRIVATE -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM)
