
add_executable(AstralEditor EditorModule.cpp
        Panels/ViewportPanel.cpp
        Panels/ViewportPanel.h
        Panels/SceneHierarchyPanel.cpp
        Panels/SceneHierarchyPanel.h
        Editor.cpp
        Editor.h
        Components/MenuBarComponent.cpp
        Components/MenuBarComponent.h
        Panels/PropertiesPanel.cpp
        Panels/PropertiesPanel.h
        Panels/EnvironmentPanel.cpp
        Utilities/AssetFileHelpers.cpp
        Components/ExternalSceneBreakdownConfigComponent.cpp
        Panels/MaterialEditorPanel.cpp
        Components/CameraController.cpp
        Panels/SettingsPanel.cpp
        Panels/SettingsPanel.h
        Components/Settings/AssetStatsComponents.cpp
        Components/Settings/AssetStatsComponents.h
        Components/Settings/BuildConfigComponents.cpp
        Components/Settings/BuildConfigComponents.h
        Components/Settings/ECSComponents.cpp
        Components/Settings/ECSComponents.h
        Components/Settings/EngineComponents.cpp
        Components/Settings/EngineComponents.h
        Components/Settings/InputStateComponents.cpp
        Components/Settings/InputStateComponents.h
        Components/Settings/MemoryComponents.cpp
        Components/Settings/MemoryComponents.h
        Components/Settings/RendererComponents.cpp
        Components/Settings/RendererComponents.h
        Components/Settings/SystemInfoComponents.cpp
        Components/Settings/SystemInfoComponents.h
        Components/Settings/WindowComponents.cpp
        Components/Settings/WindowComponents.h

)

target_link_libraries(AstralEditor AstralEngine)
target_link_libraries(AstralEditor ImGui)
target_link_libraries(AstralEditor ImGuizmo)

target_include_directories(AstralEditor PRIVATE .)

target_compile_definitions(AstralEditor PUBLIC -DASTRAL_EDITOR_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(AstralEditor PUBLIC -DCHESS_ASSET_DIR="${CMAKE_SOURCE_DIR}/Samples/Chess/Assets/")

add_dependencies(AstralEditor
    AstralProfiler
)
