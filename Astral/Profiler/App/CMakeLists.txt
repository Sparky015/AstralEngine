add_executable(AstralProfilerApp
    Components/MenuComponent.cpp
    Components/MenuComponent.h
    Components/SceneMetricsGraphComponents.cpp
    Components/SceneMetricsGraphComponents.h

    Panels/AllocationsListPanel.cpp
    Panels/AllocationsListPanel.h
    Panels/AllocationStacktracePanel.cpp
    Panels/AllocationStacktracePanel.h
    Panels/DataPointInfoPanel.cpp
    Panels/DataPointInfoPanel.h
    Panels/GraphSelectorPanel.cpp
    Panels/GraphSelectorPanel.h
    Panels/MemoryGraphPanel.cpp
    Panels/MemoryGraphPanel.h

    MemoryDebugWindow.cpp
    MemoryDebugWindow.h
    ProfilerAppModule.cpp
    ProfilerApp.cpp
    ProfilerApp.h

    Views/GraphViews.cpp
    Views/GraphViews.h
)


# Linking Libraries
target_link_libraries(AstralProfilerApp PUBLIC AstralEngine)


target_link_libraries(AstralProfilerApp PRIVATE ImPlot)
target_link_libraries(AstralProfilerApp PRIVATE nfd)


if (APPLE)
    target_link_libraries(AstralProfilerApp PRIVATE -lc++)
endif ()

add_dependencies(AstralProfilerApp
    AstralEngine
    ImPlot
    nfd
)


# Include Directories

target_include_directories(AstralProfilerApp PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/msgpack/include")
target_include_directories(AstralProfilerApp PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/cpptrace/include")
target_include_directories(AstralProfilerApp PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/nativefiledialog-extended/src/include")



# Defining Macros

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")    # DEBUG
    target_compile_definitions(AstralProfilerApp PUBLIC
            -DASTRAL_DEBUG_BUILD
    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")    # Release
    target_compile_definitions(AstralProfilerApp PUBLIC
            -DASTRAL_RELEASE_BUILD

    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")    # MinSizeRel
    target_compile_definitions(AstralProfilerApp PUBLIC
            -DASTRAL_MINIMUM_SIZE_RELEASE_BUILD

    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")    # RelWithDebInfo
    target_compile_definitions(AstralProfilerApp PUBLIC
            -DASTRAL_RELEASE_WITH_DEBUG_INFO_BUILD

    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Default")    # Default
    target_compile_definitions(AstralProfilerApp PUBLIC
            -DASTRAL_DEBUG_BUILD

    )

endif()

target_compile_definitions(AstralProfilerApp PUBLIC -DLOG_FILE_DIR="${CMAKE_SOURCE_DIR}/Logs/")
target_compile_definitions(AstralProfilerApp PUBLIC -DASTRAL_PROFILER_APP_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")