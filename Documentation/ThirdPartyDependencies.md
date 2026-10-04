
### Dependencies and Third Party Libraries

-----

- GLFW: For a cross-platform window
- GLAD: For loading OpenGL
- cpuinfo: For detecting the cpu name, amount of cores, and other info
- glm: For math
- googletest: For unit testing
- imgui: For debug menus and the editor UI
- ImPlot: For graphing memory profiling data
- msgpack: For serialization and deserialization
- stb_image: For loading image files
- cpptrace: For generating stacktraces (temporary, waiting for C++23 stacktraces to be implemented)
- nativefiledialog-extended: For native file dialog windows on macOS and Windows
- Half: For half float support
- gli: For loading .ktx and .dds files
- ImGuizmo: For gizmos in the editor for things like moving entities or rotating them
- yaml-cpp: For saving things like material files to disk (scene files in the future)
