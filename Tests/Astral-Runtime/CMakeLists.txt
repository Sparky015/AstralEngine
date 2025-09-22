

#Enabling the tests for CMake
enable_testing()

set(TEST_FILES
        ExampleTest.cpp
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

        Renderer/RHI/Resources/IndexBufferTest.cpp
        Renderer/RHI/Resources/DescriptorSetTest.cpp
)



add_executable(AstralEngineTests
        main.cpp
        ${TEST_FILES}
)

target_compile_definitions(AstralEngine PUBLIC
        -DTURN_OFF_LOGGING_CONSOLE_TO_FILE
        -DTURN_OFF_PROFILER_MACRO
)


target_link_libraries(AstralEngineTests GTest::gtest)
target_link_libraries(AstralEngineTests AstralEngine)

target_include_directories(AstralEngineTests PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/googletest-main/googletest/include/)
target_include_directories(AstralEngineTests PUBLIC ${CMAKE_SOURCE_DIR}/Source)

include(GoogleTest)
gtest_discover_tests(AstralEngineTests)

