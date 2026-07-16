

#Enabling the tests for CMake
enable_testing()

add_executable(AstralCoreTests
        Events/EventSystemTest.cpp

        Memory/Allocators/STLAllocators/STLStackAllocatorTests.cpp
        Memory/Allocators/StackAllocatorTest.cpp
        Memory/Allocators/PoolAllocatorTest.cpp
        Memory/Allocators/STLAllocators/STLLinearAllocatorTests.cpp
        Memory/Allocators/LinearAllocatorTest.cpp
        Memory/Allocators/AllocatorUtilsTest.cpp
        Memory/Allocators/RingAllocatorTest.cpp
        Memory/Pools/ObjectPoolTest.cpp
        Memory/Allocators/TLSFAllocatorTest.cpp

        Time/ClockTest.cpp
        Time/DeltaTimeTest.cpp
)


target_link_libraries(AstralCoreTests GTest::gtest_main)
target_link_libraries(AstralCoreTests AstralEngine)
target_include_directories(AstralCoreTests PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/googletest-main/googletest/include/)
target_include_directories(AstralCoreTests PUBLIC ${CMAKE_SOURCE_DIR}/Astral/Core/)

include(GoogleTest)
gtest_discover_tests(AstralCoreTests)

