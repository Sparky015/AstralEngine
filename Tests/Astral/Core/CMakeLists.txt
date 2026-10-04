

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
        Memory/Allocators/SlabAllocatorTest.cpp
        Memory/Allocators/TLSFAllocatorTest.cpp

        Time/ClockTest.cpp
        Time/DeltaTimeTest.cpp
)


target_link_libraries(AstralCoreTests GTest::gtest_main)
target_link_libraries(AstralCoreTests AstralCore)
target_include_directories(AstralCoreTests PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/googletest-main/googletest/include/)
target_include_directories(AstralCoreTests PUBLIC ${CMAKE_SOURCE_DIR}/Astral/Core/)
target_include_directories(AstralCoreTests PUBLIC ${CMAKE_SOURCE_DIR}/Astral/)

include(GoogleTest)
gtest_discover_tests(AstralCoreTests DISCOVERY_TIMEOUT 60 DISCOVERY_MODE PRE_TEST)

