

#Enabling the tests for CMake
enable_testing()

set(TEST_FILES_NO_GPU_REQUIRED
        Input/KeycodeConversionsTest.cpp
        Renderer/Cameras/CameraTest.cpp
)

set(TEST_FILES_GPU_REQUIRED
        Renderer/RHI/Resources/IndexBufferTest.cpp
        Renderer/RHI/Resources/DescriptorSetTest.cpp
        Renderer/RHI/Resources/VertexBufferTest.cpp
        Renderer/RHI/Resources/TextureTest.cpp
        Asset/AssetRegistryTest.cpp
        Asset/AssetRegistryMaterialLoaderTest.cpp
        Asset/AssetRegistryMeshLoaderTest.cpp
        Asset/AssetRegistryShaderLoaderTest.cpp
        Asset/AssetRegistryEnvironmentMapLoaderTest.cpp
        Asset/AssetRegistryCubeLUTLoaderTest.cpp
        Scenes/SceneLoaderTest.cpp
        Renderer/RHI/Resources/ShaderTest.cpp
        Renderer/RHI/Resources/ComputePipelineTest.cpp
        Renderer/RHI/Resources/GraphicsPipelineTest.cpp
        Renderer/RHI/Resources/BufferTest.cpp
)


add_executable(AstralRuntimeTestsWithoutGPU
        ${TEST_FILES_NO_GPU_REQUIRED}
)

add_executable(AstralRuntimeTestsWithGPU
        main.cpp
        ${TEST_FILES_NO_GPU_REQUIRED}
        ${TEST_FILES_GPU_REQUIRED}
)




target_link_libraries(AstralRuntimeTestsWithoutGPU GTest::gtest_main)
target_link_libraries(AstralRuntimeTestsWithoutGPU AstralEngine)
target_include_directories(AstralRuntimeTestsWithoutGPU PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/googletest-main/googletest/include/)
target_include_directories(AstralRuntimeTestsWithoutGPU PUBLIC ${CMAKE_SOURCE_DIR}/Source)
target_compile_definitions(AstralRuntimeTestsWithoutGPU PUBLIC  # These don't work for main astral library
        -DTURN_OFF_LOGGING_CONSOLE_TO_FILE
        -DTURN_OFF_PROFILER_MACRO
)

target_link_libraries(AstralRuntimeTestsWithGPU GTest::gtest)
target_link_libraries(AstralRuntimeTestsWithGPU AstralEngine)
target_include_directories(AstralRuntimeTestsWithGPU PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/googletest-main/googletest/include/)
target_include_directories(AstralRuntimeTestsWithGPU PUBLIC ${CMAKE_SOURCE_DIR}/Source)
target_compile_definitions(AstralRuntimeTestsWithGPU PUBLIC  # These don't work for main astral library
        -DTURN_OFF_LOGGING_CONSOLE_TO_FILE
        -DTURN_OFF_PROFILER_MACRO
)
target_compile_definitions(AstralRuntimeTestsWithGPU PRIVATE -DASTRAL_TEST_ASSETS_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/Asset/TestAssets")
target_compile_definitions(AstralRuntimeTestsWithGPU PRIVATE -DASTRAL_TEST_SCENES_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/Scenes/TestScenes")



include(GoogleTest)
gtest_discover_tests(AstralRuntimeTestsWithoutGPU)

