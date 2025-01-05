

#Enabling the tests for CMake
enable_testing()

set(TEST_FILES
        ExampleTest.cpp
        Core/Events/EventSystemTest.cpp
        Input/KeycodeConversionsTest.cpp
)


add_executable(AstralEngineTests
        ${TEST_FILES}
)

target_link_libraries(AstralEngineTests GTest::gtest_main)
target_link_libraries(AstralEngineTests AstralEngine)

target_include_directories(AstralEngineTests PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/googletest-main/googletest/include/)
target_include_directories(AstralEngineTests PUBLIC ${CMAKE_SOURCE_DIR}/Source)

include(GoogleTest)
gtest_discover_tests(AstralEngineTests)