
add_library(Astral_ImGuiManager

        Private/ImGuiDependencies/imgui_impl_glfw.cpp
        Private/ImGuiDependencies/imgui_impl_glfw.h
        Private/ImGuiDependencies/imgui_impl_opengl3.cpp
        Private/ImGuiDependencies/imgui_impl_opengl3.h

        Public/ImGuiEvents.h
        Public/ImGuiManager.cpp
        Public/ImGuiManager.h
)

target_include_directories(Astral_ImGuiManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Public)
target_include_directories(Astral_ImGuiManager PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Private)

target_link_libraries(Astral_ImGuiManager PRIVATE ImGui)

add_dependencies(Astral_ImGuiManager ImGui)