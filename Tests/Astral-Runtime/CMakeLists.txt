

#Enabling the tests for CMake
enable_testing()

set(TEST_FILES_NO_GPU_REQUIRED
        Core/Events/EventSystemTest.cpp
        Input/KeycodeConversionsTest.cpp

        Core/Memory/Allocators/STLAllocators/STLStackAllocatorTests.cpp
        Core/Memory/Allocators/STLAllocators/STLStackBasedLinearAllocatorTests.cpp
        Core/Memory/Allocators/StackBasedLinearAllocatorTest.cpp
        Core/Memory/Allocators/StackAllocatorTest.cpp
        Core/Memory/Allocators/FrameAllocatorTest.cpp
        Core/Memory/Allocators/DoubleBufferedAllocatorTest.cpp
        Core/Memory/Allocators/PoolAllocatorTest.cpp
        Core/Memory/Pools/ObjectPoolTest.cpp
        Core/Memory/Allocators/STLAllocators/STLLinearAllocatorTests.cpp
        Core/Memory/Allocators/LinearAllocatorTest.cpp
        Core/Memory/Allocators/AllocatorUtilsTest.cpp
        Core/Memory/Allocators/RingAllocatorTest.cpp

        Renderer/Cameras/CameraTest.cpp
        Debug/MemoryTracking/GlobalAllocationStorageTest.cpp
        Debug/MemoryTracking/SceneMetricsExporterTest.cpp
)

set(TEST_FILES_GPU_REQUIRED
        Renderer/RHI/Resources/IndexBufferTest.cpp
        Renderer/RHI/Resources/DescriptorSetTest.cpp
        Renderer/RHI/Resources/FramebufferTest.cpp
        Renderer/RHI/Resources/VertexBufferTest.cpp
        Renderer/RHI/Resources/TextureTest.cpp
        Asset/AssetRegistryTest.cpp
        Asset/AssetRegistryMaterialLoaderTest.cpp
        Asset/AssetRegistryMeshLoaderTest.cpp
        Asset/AssetRegistryShaderLoaderTest.cpp
        Asset/AssetRegistryEnvironmentMapLoaderTest.cpp
        Asset/AssetRegistryCubeLUTLoaderTest.cpp
        Scenes/SceneLoaderTest.cpp
        Core/Time/ClockTest.cpp
        Core/Time/DeltaTimeTest.cpp
)


add_executable(AstralEngineTests
        ${TEST_FILES_NO_GPU_REQUIRED}
)

add_executable(AstralEngineTestsWithGPU
        main.cpp
        ${TEST_FILES_NO_GPU_REQUIRED}
        ${TEST_FILES_GPU_REQUIRED}
)




target_link_libraries(AstralEngineTests GTest::gtest_main)
target_link_libraries(AstralEngineTests AstralEngine)
target_include_directories(AstralEngineTests PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/googletest-main/googletest/include/)
target_include_directories(AstralEngineTests PUBLIC ${CMAKE_SOURCE_DIR}/Source)


target_link_libraries(AstralEngineTestsWithGPU GTest::gtest)
target_link_libraries(AstralEngineTestsWithGPU AstralEngine)
target_include_directories(AstralEngineTestsWithGPU PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/googletest-main/googletest/include/)
target_include_directories(AstralEngineTestsWithGPU PUBLIC ${CMAKE_SOURCE_DIR}/Source)

target_compile_definitions(AstralEngineTestsWithGPU PRIVATE -DASTRAL_TEST_ASSETS_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/Asset/TestAssets/")
target_compile_definitions(AstralEngineTestsWithGPU PRIVATE -DASTRAL_TEST_SCENES_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/Scenes/TestScenes/")



include(GoogleTest)
gtest_discover_tests(AstralEngineTests)

