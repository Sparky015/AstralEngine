
add_library(ImPlot
    implot.cpp
    implot.h
    implot_internal.h
    implot_items.cpp
    implot_demo.cpp
)

target_link_libraries(ImPlot PRIVATE ImGui)

target_include_directories(ImPlot PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})