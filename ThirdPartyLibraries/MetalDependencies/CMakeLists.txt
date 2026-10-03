
message("Finding Metal dependencies...")

find_library(METAL_FRAMEWORK Metal)
find_library(FOUNDATION_FRAMEWORK Foundation)
find_library(QUARTZCORE_FRAMEWORK QuartzCore)

if (METAL_FRAMEWORK)
    message(STATUS "Found Metal Framework! (${METAL_FRAMEWORK})")
else ()
    message(STATUS "Metal Framework not found!")
endif ()

if (FOUNDATION_FRAMEWORK)
    message(STATUS "Found Foundation Framework! (${FOUNDATION_FRAMEWORK})")
else ()
    message(STATUS "Foundation Framework not found!")
endif ()

if (QUARTZCORE_FRAMEWORK)
    message(STATUS "Found QuartzCore Framework! (${QUARTZCORE_FRAMEWORK})")
else ()
    message(STATUS "QuartzCore Framework not found!")
endif ()


# Only build the Metal4 target if the framework dependencies are found
if (METAL_FRAMEWORK AND FOUNDATION_FRAMEWORK AND QUARTZCORE_FRAMEWORK)
    add_library(Metal4
        HeaderLibraryImplementation.cpp
    )

    target_include_directories(Metal4 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(Metal4 ${METAL_FRAMEWORK} ${FOUNDATION_FRAMEWORK} ${QUARTZCORE_FRAMEWORK} spirv-cross)
    message(STATUS "All Metal dependencies were found!")
    message(STATUS "Metal is supported!")

else ()
    message(STATUS "One or more dependencies of Metal was not found!")
    message(STATUS "Metal is not supported!")
endif ()

message("") # Spacing