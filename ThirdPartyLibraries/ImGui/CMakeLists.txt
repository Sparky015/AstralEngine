
add_library(ImGui STATIC imgui.cpp
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_widgets.cpp
        imgui_tables.cpp
        imgui.h
        imconfig.h
        imgui_internal.h
)

target_include_directories(ImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})