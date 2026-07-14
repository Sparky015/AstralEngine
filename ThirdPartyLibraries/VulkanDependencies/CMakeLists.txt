

message("Finding Vulkan dependencies...")


# Only build the Vulkan target if the framework dependencies are found
if (DEFINED ENV{VULKAN_SDK} AND NOT "$ENV{VULKAN_SDK}" STREQUAL "")
    message(STATUS "Found Vulkan SDK! ($ENV{VULKAN_SDK})")
    message(STATUS "Vulkan is supported!")
    add_library(VulkanDependencies INTERFACE)

    target_include_directories(VulkanDependencies INTERFACE "$ENV{VULKAN_SDK}/include")

else ()
    message(STATUS "Vulkan SDK not found!")
    message(STATUS "Vulkan is not supported!")
endif ()

message("") # Spacing


if (WIN32 AND TARGET VulkanDependencies)
    target_link_directories(VulkanDependencies INTERFACE "$ENV{VULKAN_SDK}/Lib")
elseif (APPLE AND TARGET VulkanDependencies)
    target_link_directories(VulkanDependencies INTERFACE "$ENV{VULKAN_SDK}/lib")
endif()


if (WIN32 AND TARGET VulkanDependencies)
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        target_link_libraries(VulkanDependencies INTERFACE
            vulkan-1
            SPIRV-Toolsd
            SPIRV-Tools-diffd
            SPIRV-Tools-optd
            glslangd
            glslang-default-resource-limitsd
        )
    else ()
        target_link_libraries(VulkanDependencies INTERFACE
            vulkan-1
            glslang
            glslang-default-resource-limits
            SPIRV-Tools
            SPIRV-Tools-diff
            SPIRV-Tools-opt
        )
    endif ()

elseif (APPLE AND TARGET VulkanDependencies)
    target_link_libraries(VulkanDependencies INTERFACE
            vulkan.1
            glslang
            glslang-default-resource-limits
            SPIRV-Tools
            SPIRV-Tools-diff
            SPIRV-Tools-opt
    )
endif ()