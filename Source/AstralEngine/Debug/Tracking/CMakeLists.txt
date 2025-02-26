
add_library(Astral_MemoryTracker

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
