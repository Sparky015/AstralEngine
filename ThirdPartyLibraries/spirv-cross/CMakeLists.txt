add_library(spirv-cross INTERFACE)

target_link_libraries(spirv-cross INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libspirv-cross-core.a
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libspirv-cross-cpp.a
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libspirv-cross-glsl.a
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libspirv-cross-msl.a
)

target_include_directories(spirv-cross INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
