add_library(AstralProfilerAPI
        Instrumentation/ProfilerLogFile.cpp
        Instrumentation/ProfilerLogFile.h
        Instrumentation/ScopeProfiler.cpp
        Instrumentation/ScopeProfiler.h

        MemoryTracking/AllocationData.h
        MemoryTracking/GlobalAllocationStorage.cpp
        MemoryTracking/GlobalAllocationStorage.h
        MemoryTracking/MemoryMetrics.cpp
        MemoryTracking/MemoryMetrics.h
        MemoryTracking/MemoryTracker.cpp
        MemoryTracking/MemoryTracker.h
        MemoryTracking/NoTrackingSTLAllocator.h

        MemoryTracking/Hooks/GenericRegionAllocator.cpp
        MemoryTracking/Hooks/GenericRegionAllocator.h
        MemoryTracking/Hooks/MallocFreeOverrides.cpp
        MemoryTracking/Hooks/MallocFreeOverrides.h
        MemoryTracking/Hooks/NewDeleteOverrides.cpp
        MemoryTracking/Hooks/NewDeleteOverrides.h

        MemoryTracking/Serialization/MemoryMetricsSnapshot.h
        MemoryTracking/Serialization/SceneMetricsExporter.cpp
        MemoryTracking/Serialization/SceneMetricsExporter.h
        MemoryTracking/Serialization/SceneMetricsImporter.cpp
        MemoryTracking/Serialization/SceneMetricsImporter.h
        MemoryTracking/Serialization/SceneMetricsStorage.cpp
        MemoryTracking/Serialization/SceneMetricsStorage.h
)


# Linking Libraries
target_link_libraries(AstralProfilerAPI PUBLIC AstralCore)
target_link_libraries(AstralProfilerAPI PUBLIC msgpack-cxx)
target_link_libraries(AstralProfilerAPI PRIVATE cpptrace::cpptrace)
target_compile_definitions(AstralProfilerAPI PRIVATE -DCPPTRACE_STATIC_DEFINE)



if (APPLE)
    target_link_libraries(AstralProfilerAPI PRIVATE -lc++)
endif ()

add_dependencies(AstralProfilerAPI
    AstralCore
    nfd
    cpptrace::cpptrace
    msgpack-cxx
)


# Include Directories

target_include_directories(AstralProfilerAPI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")

target_include_directories(AstralProfilerAPI PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/msgpack/include")
target_include_directories(AstralProfilerAPI PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/cpptrace/include")
target_include_directories(AstralProfilerAPI PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/nativefiledialog-extended/src/include")


# Defining Macros

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")    # DEBUG
    target_compile_definitions(AstralProfilerAPI PUBLIC
            -DASTRAL_DEBUG_BUILD
    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")    # Release
    target_compile_definitions(AstralProfilerAPI PUBLIC
            -DASTRAL_RELEASE_BUILD

    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")    # MinSizeRel
    target_compile_definitions(AstralProfilerAPI PUBLIC
            -DASTRAL_MINIMUM_SIZE_RELEASE_BUILD
    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")    # RelWithDebInfo
    target_compile_definitions(AstralProfilerAPI PUBLIC
            -DASTRAL_RELEASE_WITH_DEBUG_INFO_BUILD

    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Default")    # Default
    target_compile_definitions(AstralProfilerAPI PUBLIC
            -DASTRAL_DEBUG_BUILD

    )

endif()

target_compile_definitions(AstralProfilerAPI PUBLIC -DLOG_FILE_DIR="${CMAKE_SOURCE_DIR}/Logs/")