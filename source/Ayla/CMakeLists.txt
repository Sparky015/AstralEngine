
# Create the static library
add_library(Ayla STATIC
        Core/Application.cpp
        Events/Event.cpp
        Events/EventDispatcher.cpp
        aypch.cpp
        Window/Platform/Generic/GenericWindow.cpp
        Core/Layers/Layer.cpp
        Core/Layers/Layer.h
        Core/Layers/LayerStack.cpp
        ImGui/ImGuiLayer.cpp
        Input/InputLayer.cpp
        Input/InputState.cpp
        Debug/DebugLayer.cpp
        Core/Time/Clock.cpp
        Input/KeycodeTranslation/GLFWTranslation.cpp
        Renderer/Platform/OpenGL/imgui_impl_opengl3.cpp
        Window/Platform/Generic/imgui_impl_glfw.cpp
        Client/UserSetup.cpp
        Core/Time/FTimer.cpp
        Debug/DebugMacros.h
        Debug/DebugMacros.cpp
)

#       ### Ayla Includes ###
target_include_directories(Ayla PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(Ayla PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_sources(Ayla PUBLIC
        ${CMAKE_SOURCE_DIR}/source/Ayla/Core/Layers/Layer.h
        ${CMAKE_SOURCE_DIR}/include/Ayla.h
)

#           Include library headers
target_include_directories(Ayla PRIVATE ${CMAKE_SOURCE_DIR}/vendor/GLFW/include)
target_include_directories(Ayla PRIVATE ${CMAKE_SOURCE_DIR}/vendor/glm)



#           Link Libraries to Ayla
target_link_libraries(Ayla GLAD)
target_link_libraries(Ayla ImGui)
if (APPLE)
    target_link_libraries(Ayla glfw.3)
elseif (WIN32)
    target_link_libraries(Ayla glfw3)
endif ()

# Link directories for Cmake (Point to where the lib files are for GLFW)
if (APPLE)
    target_link_directories(Ayla PRIVATE ${CMAKE_SOURCE_DIR}/vendor/GLFW/mac-lib)
elseif (WIN32)
    target_link_directories(Ayla PUBLIC ${CMAKE_SOURCE_DIR}/vendor/GLFW/windows-lib)
endif ()


# Defining Macros
target_compile_definitions(Ayla PRIVATE -DGLFW_INCLUDE_NONE)
target_compile_definitions(Ayla PRIVATE -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM)


# Defining Precompiled Headers
target_precompile_headers(Ayla PRIVATE ${CMAKE_SOURCE_DIR}/source/Ayla/aypch.h)

