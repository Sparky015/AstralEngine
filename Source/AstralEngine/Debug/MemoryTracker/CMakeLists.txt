
add_library(Astral_MemoryTracker

        Public/MemoryTracker.cpp
        Public/MemoryTracker.h

        Private/Visualization/Components/LoadFileComponent.cpp
        Private/Visualization/Components/LoadFileComponent.h
        Private/Visualization/Components/SceneMetricsGraphComponents.cpp
        Private/Visualization/Components/SceneMetricsGraphComponents.h
        Private/Visualization/Panels/AllocationsListPanel.cpp
        Private/Visualization/Panels/AllocationsListPanel.h
        Private/Visualization/Panels/AllocationStacktracePanel.cpp
        Private/Visualization/Panels/AllocationStacktracePanel.h
        Private/Visualization/Panels/DataPointInfoPanel.cpp
        Private/Visualization/Panels/DataPointInfoPanel.h
        Private/Visualization/Panels/GraphSelectorPanel.cpp
        Private/Visualization/Panels/GraphSelectorPanel.h
        Private/Visualization/Panels/MemoryGraphPanel.cpp
        Private/Visualization/Panels/MemoryGraphPanel.h
        Private/Visualization/MemoryDebugWindow.cpp
        Private/Visualization/MemoryDebugWindow.h

        Private/Serialization/SceneMetricsExporter.cpp
        Private/Serialization/SceneMetricsExporter.h
        Private/Serialization/SceneMetricsImporter.cpp
        Private/Serialization/SceneMetricsImporter.h
        Private/Serialization/SceneMetricsStorage.cpp
        Private/Serialization/SceneMetricsStorage.h
        Private/Serialization/MemoryMetricsSnapshot.h

        Private/GlobalAllocationStorage.cpp
        Private/GlobalAllocationStorage.h
        Private/MemoryMetrics.cpp
        Private/MemoryMetrics.h
        Private/NewDeleteOverrides.cpp
        Private/NewDeleteOverrides.h
)

target_include_directories(Astral_MemoryTracker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Public)
target_include_directories(Astral_MemoryTracker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Private)


target_link_libraries(Astral_MemoryTracker PRIVATE ImGui)
target_link_libraries(Astral_MemoryTracker PRIVATE ImPlot)
target_link_libraries(Astral_MemoryTracker PRIVATE ImGuiFileDialog)
target_link_libraries(Astral_MemoryTracker PRIVATE msgpack-cxx)
target_link_libraries(Astral_MemoryTracker PRIVATE cpptrace::cpptrace)
target_link_libraries(Astral_MemoryTracker PRIVATE Astral_ImGuiManager)


add_dependencies(Astral_MemoryTracker
        ImGui
        ImPlot
        ImGuiFileDialog
        msgpack-cxx
        cpptrace::cpptrace
        Astral_ImGuiManager
)


target_compile_definitions(Astral_MemoryTracker PRIVATE -DCPPTRACE_STATIC_DEFINE)
