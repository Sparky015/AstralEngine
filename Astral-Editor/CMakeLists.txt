
add_executable(AstralEditor EditorModule.cpp
        ViewportPanel.cpp
        ViewportPanel.h
        SceneGraphPanel.cpp
        SceneGraphPanel.h
        Editor.cpp
        Editor.h
        ChessEntities.h
        ChessEntities.cpp)

target_link_libraries(AstralEditor AstralEngine)
target_link_libraries(AstralEditor ImGui)

target_compile_definitions(AstralEditor PUBLIC -DASTRAL_EDITOR_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")
target_compile_definitions(AstralEditor PUBLIC -DCHESS_ASSET_DIR="${CMAKE_SOURCE_DIR}/Samples/Chess/Assets/")
