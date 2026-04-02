add_library(AstralCore
    Containers/Bitmap.h
    Containers/Bitmap_Impl.h
    Containers/DirectedGraph.cpp
    Containers/DirectedGraph.h
    Containers/DirectedGraphEdge.h
    Containers/DirectedGraphVertex.h

    Events/EventBus.h
    Events/EventListener.h
    Events/EventPublisher.h

    Math/Math.h

    Memory/Allocators/STLAllocators/STLLinearAllocator.h
    Memory/Allocators/STLAllocators/STLStackAllocator.h
    Memory/Allocators/STLAllocators/STLStackBasedLinearAllocator.h
    Memory/Allocators/AlignedAllocator.cpp
    Memory/Allocators/AlignedAllocator.h
    Memory/Allocators/AllocatorUtils.cpp
    Memory/Allocators/AllocatorUtils.h
    Memory/Allocators/DoubleBufferedAllocator.cpp
    Memory/Allocators/DoubleBufferedAllocator.h
    Memory/Allocators/FrameAllocator.cpp
    Memory/Allocators/FrameAllocator.h
    Memory/Allocators/LinearAllocation.cpp
    Memory/Allocators/LinearAllocator.h
    Memory/Allocators/PoolAllocator.cpp
    Memory/Allocators/PoolAllocator.h
    Memory/Allocators/RingAllocator.cpp
    Memory/Allocators/RingAllocator.h
    Memory/Allocators/StackAllocator.cpp
    Memory/Allocators/StackAllocator.h
    Memory/Allocators/StackBasedLinearAllocator.h
    Memory/Allocators/StackBasedLinearAllocator_impl.h
    Memory/Pools/ObjectPool.h

    Time/Clock.cpp
    Time/Clock.h
    Time/DeltaTime.cpp
    Time/DeltaTime.h

    Utilities/Asserts.cpp
    Utilities/Asserts.h
    Utilities/ConsoleColors.cpp
    Utilities/ConsoleColors.h
    Utilities/ConsoleLogFile.cpp
    Utilities/ConsoleLogFile.h
    Utilities/Error.cpp
    Utilities/Error.h
    Utilities/HashCombiner.cpp
    Utilities/HashCombiner.h
    Utilities/Loggers.cpp
    Utilities/Loggers.h

    FixedIntegerTypes.h
    Containers/DirectedGraphHashes.h
)


# Flags for just the Astral Engine and all libraries and executables that link to it
if (APPLE)
    target_compile_options(AstralCore PRIVATE -Wall -Wextra)#-fsanitize=address
else ()
    target_compile_options(AstralCore PRIVATE -W1)
endif()


# Linking Libraries

target_link_libraries(AstralCore PRIVATE glm)
target_link_libraries(AstralCore PRIVATE cpptrace::cpptrace)
target_compile_definitions(AstralCore PRIVATE -DCPPTRACE_STATIC_DEFINE)


if (APPLE)
    target_link_libraries(AstralCore PRIVATE -lc++)
endif ()

add_dependencies(AstralCore
    cpptrace::cpptrace
    glm
)




# Include Directories
target_include_directories(AstralCore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_include_directories(AstralCore PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_include_directories(AstralCore PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/glm")
target_include_directories(AstralCore PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/cpptrace/include")


# Defining Macros

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")    # DEBUG
    target_compile_definitions(AstralCore PUBLIC -DASTRAL_DEBUG_BUILD
    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")    # Release
    target_compile_definitions(AstralCore PUBLIC
        -DASTRAL_RELEASE_BUILD
        -DTURN_OFF_DEBUG_MACROS
        -DTURN_OFF_PROFILER_MACRO
        -DTURN_OFF_LOGGING_CONSOLE_TO_FILE
    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")    # MinSizeRel
    target_compile_definitions(AstralCore PUBLIC
            -DASTRAL_MINIMUM_SIZE_RELEASE_BUILD
    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")    # RelWithDebInfo
    target_compile_definitions(AstralCore PUBLIC
            -DASTRAL_RELEASE_WITH_DEBUG_INFO_BUILD
            -DTURN_OFF_LOGGING_CONSOLE_TO_FILE
#            -DTURN_OFF_DEBUG_MACROS
    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Default")    # Default
    target_compile_definitions(AstralEngine PUBLIC
            -DASTRAL_DEBUG_BUILD
    )

endif()



if (APPLE)
    target_compile_definitions(AstralCore PUBLIC -DASTRAL_PLATFORM_MACOS)

elseif (WIN32)
    target_compile_definitions(AstralCore PUBLIC -DASTRAL_PLATFORM_WINDOWS)

elseif (LINUX)
    target_compile_definitions(AstralCore PUBLIC -DASTRAL_PLATFORM_LINUX)

endif ()

target_compile_definitions(AstralCore PUBLIC -DROOT_DIR="${CMAKE_SOURCE_DIR}/")
target_compile_definitions(AstralCore PUBLIC -DLOG_FILE_DIR="${CMAKE_SOURCE_DIR}/Logs/")
target_compile_definitions(AstralCore PUBLIC -DENGINE_BUILD_PROFILE="${CMAKE_BUILD_TYPE}")