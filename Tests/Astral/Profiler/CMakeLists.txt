

#Enabling the tests for CMake
enable_testing()

add_executable(AstralProfilerTests
    Library/MemoryTracking/GlobalAllocationStorageTest.cpp
    Library/MemoryTracking/SceneMetricsExporterTest.cpp
)


target_link_libraries(AstralProfilerTests GTest::gtest_main)
target_link_libraries(AstralProfilerTests AstralProfilerAPI)
target_include_directories(AstralProfilerTests PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/googletest-main/googletest/include/)
target_include_directories(AstralProfilerTests PUBLIC ${CMAKE_SOURCE_DIR}/Astral/Profiler/Library/)

include(GoogleTest)
gtest_discover_tests(AstralProfilerTests)

