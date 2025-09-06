add_library(ImGuizmo STATIC
    GraphEditor.cpp
    GraphEditor.h
    ImCurveEdit.cpp
    ImCurveEdit.h
    ImGradient.cpp
    ImGradient.h
    ImGuizmo.cpp
    ImGuizmo.h
    ImSequencer.cpp
    ImSequencer.h
    ImZoomSlider.h
)

target_link_libraries(ImGuizmo PRIVATE ImGui)
target_include_directories(ImGuizmo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})