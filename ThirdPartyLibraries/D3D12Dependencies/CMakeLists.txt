
if (WIN32)

    message("Finding D3D12 dependencies...")

    find_library(D3D12_LIB d3d12)
    find_library(DXGI_LIB dxgi)
    find_path(D3D12_INCLUDE_DIR d3d12.h)


    if (D3D12_LIB)
        message(STATUS "Found D3D12 Library! (${D3D12_LIB})")
    else ()
        message(STATUS "D3D12 Library not found!")
    endif ()

    if (DXGI_LIB)
        message(STATUS "Found DXGI Library! (${DXGI_LIB})")
    else ()
        message(STATUS "DXGI Library not found!")
    endif ()

    if (D3D12_INCLUDE_DIR)
        message(STATUS "Found D3D12 Include Path! (${D3D12_INCLUDE_DIR})")
    else ()
        message(STATUS "D3D12 Include Path not found!")
    endif ()


    # Only build the D3D12 target if the framework dependencies are found
    if (D3D12_LIB AND DXGI_LIB AND D3D12_INCLUDE_DIR)
        add_library(D3D12Dependencies INTERFACE)

        target_link_libraries(D3D12Dependencies INTERFACE ${D3D12_LIB})
        target_link_libraries(D3D12Dependencies INTERFACE ${DXGI_LIB})
        target_include_directories(D3D12Dependencies INTERFACE ${D3D12_INCLUDE_DIR})

        message(STATUS "All D3D12 dependencies were found!")
        message(STATUS "D3D12 is supported!")

    else ()
        message(STATUS "One or more dependencies of D3D12 was not found!")
        message(STATUS "D3D12 is not supported!")
    endif ()

    message("") # Spacing

endif ()