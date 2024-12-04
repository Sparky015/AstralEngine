

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
        Game/Pieces/King.h
        Game/Pieces/King.cpp
        Game/Pieces/Knight.cpp
        Game/Pieces/Pawn.cpp
        Game/Pieces/Queen.cpp
        Game/Pieces/Rook.cpp
        Game/MoveList.cpp
        Game/Conversions.h
        Game/Conversions.cpp
        Game/MoveGenerationHelpers.cpp
        Game/MoveGenerationHelpers.h
        ECS/ECSManager.cpp
        ECS/ECSManager.h
        ECS/ECS.cpp
        ECS/ECS.h
        ECS/Entity.cpp
        ECS/Entity.h
        ECS/Components/Transform.h
        ECS/ECSTypes.h
        ECS/Components/Sprite.h
        Game/GameManager.h
        Game/GameManager.cpp
        Renderer/RendererManager.cpp
        Renderer/RendererManager.h
        Renderer/Platform/OpenGL/Primitives/OpenGLVertexBuffer.h
        Renderer/Platform/OpenGL/Primitives/OpenGLVertexBuffer.cpp
        Renderer/Platform/OpenGL/Primitives/OpenGLVertexArrayObject.h
        Renderer/Platform/OpenGL/Primitives/OpenGLVertexArrayObject.cpp
        Renderer/Platform/OpenGL/Primitives/OpenGLIndexBuffer.h
        Renderer/Platform/OpenGL/Primitives/OpenGLBufferLayout.cpp
        Renderer/Platform/OpenGL/Primitives/OpenGLIndexBuffer.cpp
        Renderer/Platform/OpenGL/Primitives/OpenGLBufferLayout.h
        Renderer/Platform/OpenGL/Shaders/OpenGLShaderProgram.cpp
        Renderer/Platform/OpenGL/Shaders/OpenGLShaderProgram.h
        Renderer/Shaders/ShaderSource.h
        Renderer/Shaders/ShaderSource.cpp
        Renderer/Primitives/VertexBuffer.cpp
        Renderer/Primitives/VertexBuffer.h
        Renderer/Primitives/IndexBuffer.cpp
        Renderer/Primitives/IndexBuffer.h
        Renderer/Renderer.cpp
        Renderer/Renderer.h
        Renderer/Platform/OpenGL/OpenGLRenderingContext.h
        Renderer/Platform/OpenGL/OpenGLRenderingContext.cpp
        Renderer/RendererContext.h
        Renderer/RendererContext.cpp
        Window/Window.cpp
        Window/Window.h
        Window/Platform/Generic/GenericWindow.cpp
        Window/Platform/Generic/GenericWindow.h
        Renderer/Primitives/VertexArrayObject.cpp
        Renderer/Primitives/VertexArrayObject.h
        Renderer/Texture.cpp
        Renderer/Texture.h
        Renderer/Platform/OpenGL/OpenGLTexture.cpp
        Renderer/Platform/OpenGL/OpenGLTexture.h
        Renderer/Primitives/BufferLayout.cpp
        Renderer/Primitives/BufferLayout.h
        Core/Math.h
        Renderer/Shaders/ShaderProgram.cpp
        Renderer/Shaders/ShaderProgram.h
)



add_executable(Sozin main.cpp)

target_link_libraries(Sozin PRIVATE ChessEngine)
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sozin)

if (WIN32) #MacOS doesn't like this, so I will only do it for windows
    set_target_properties(Sozin PROPERTIES LINK_FLAGS "-static")
endif()

# Link directories for Cmake (Point to where the lib files are for GLFW)
if (APPLE)
    target_link_directories(ChessEngine PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/mac-lib)
elseif (WIN32)
    target_link_directories(ChessEngine PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/windows-lib)
endif ()



# Linking Libraries
target_link_libraries(ChessEngine PRIVATE ImGui)
target_link_libraries(ChessEngine PRIVATE GLAD)
target_link_libraries(ChessEngine PRIVATE glm)
target_link_libraries(ChessEngine PRIVATE std_image)

if (APPLE)
    target_link_libraries(ChessEngine PRIVATE glfw.3)
elseif (WIN32)
    target_link_libraries(ChessEngine PRIVATE glfw3)
endif ()



# Include Directories
target_include_directories(ChessEngine PUBLIC "${CMAKE_SOURCE_DIR}/Source")
target_include_directories(ChessEngine PRIVATE "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/include")
target_include_directories(ChessEngine PRIVATE "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLAD")
target_include_directories(ChessEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/glm")



# Precompiled Header
target_precompile_headers(ChessEngine PRIVATE "${CMAKE_SOURCE_DIR}/Source/pch.h")



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
target_compile_definitions(ChessEngine PRIVATE -DSHADER_DIR="${CMAKE_SOURCE_DIR}/Source/Renderer/Shaders/")
