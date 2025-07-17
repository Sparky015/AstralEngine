
add_executable(AstralEditor EditorModule.cpp
        Panels/ViewportPanel.cpp
        Panels/ViewportPanel.h
        Panels/SceneHierarchyPanel.cpp
        Panels/SceneHierarchyPanel.h
        Editor.cpp
        Editor.h
        ChessEntities.h
        ChessEntities.cpp
        Components/MenuBarComponent.cpp
        Components/MenuBarComponent.h
        Panels/PropertiesPanel.cpp
        Panels/PropertiesPanel.h)

target_link_libraries(AstralEditor AstralEngine)
target_link_libraries(AstralEditor ImGui)
target_link_libraries(AstralEditor ImGuizmo)

target_compile_definitions(AstralEditor PUBLIC -DASTRAL_EDITOR_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")
target_compile_definitions(AstralEditor PUBLIC -DCHESS_ASSET_DIR="${CMAKE_SOURCE_DIR}/Samples/Chess/Assets/")
