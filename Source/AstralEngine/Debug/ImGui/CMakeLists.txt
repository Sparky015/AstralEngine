
add_library(Astral_ImGuiManager

        ImGuiDependencies/imgui_impl_glfw.cpp
        ImGuiDependencies/imgui_impl_glfw.h
        ImGuiDependencies/imgui_impl_opengl3.cpp
        ImGuiDependencies/imgui_impl_opengl3.h

        ImGuiEvents.h
        ImGuiManager.cpp
        ImGuiManager.h
)

target_include_directories(Astral_ImGuiManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Public)
target_include_directories(Astral_ImGuiManager PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Private)

target_link_libraries(Astral_ImGuiManager PRIVATE ImGui)

add_dependencies(Astral_ImGuiManager ImGui)