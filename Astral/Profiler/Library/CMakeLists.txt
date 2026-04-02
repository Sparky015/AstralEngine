add_library(AstralProfiler
    Instrumentation/ProfilerLogFile.cpp
    Instrumentation/ProfilerLogFile.h
)


# Linking Libraries
target_link_libraries(AstralProfiler PUBLIC AstralCore)
target_link_libraries(AstralProfiler PUBLIC msgpack-cxx)
target_link_libraries(AstralProfiler PRIVATE cpptrace::cpptrace)
target_compile_definitions(AstralProfiler PRIVATE -DCPPTRACE_STATIC_DEFINE)



if (APPLE)
    target_link_libraries(AstralProfiler PRIVATE -lc++)
endif ()

add_dependencies(AstralProfiler
    AstralCore
    nfd
    cpptrace::cpptrace
    msgpack-cxx
)


# Include Directories

target_include_directories(AstralProfiler PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/msgpack/include")
target_include_directories(AstralProfiler PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/cpptrace/include")
target_include_directories(AstralProfiler PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/nativefiledialog-extended/src/include")


# Defining Macros

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")    # DEBUG
    target_compile_definitions(AstralProfiler PUBLIC
            -DASTRAL_DEBUG_BUILD
    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")    # Release
    target_compile_definitions(AstralProfiler PUBLIC
            -DASTRAL_RELEASE_BUILD

    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")    # MinSizeRel
    target_compile_definitions(AstralProfiler PUBLIC
            -DASTRAL_MINIMUM_SIZE_RELEASE_BUILD
    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")    # RelWithDebInfo
    target_compile_definitions(AstralProfiler PUBLIC
            -DASTRAL_RELEASE_WITH_DEBUG_INFO_BUILD

    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Default")    # Default
    target_compile_definitions(AstralProfiler PUBLIC
            -DASTRAL_DEBUG_BUILD

    )

endif()

target_compile_definitions(AstralProfiler PUBLIC -DLOG_FILE_DIR="${CMAKE_SOURCE_DIR}/Logs/")