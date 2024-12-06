

#Enabling the tests for CMake
enable_testing()

set(TEST_FILES
        ExampleTest.cpp
        Core/Events/EventSystemTest.cpp
)


add_executable(
        AllTests
        ${TEST_FILES}
)

target_link_libraries(AllTests GTest::gtest_main)
target_link_libraries(AllTests ChessEngine)

target_include_directories(AllTests PRIVATE ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/googletest-main/googletest/include/)
target_include_directories(AllTests PRIVATE ${CMAKE_SOURCE_DIR}/Source)

include(GoogleTest)
gtest_discover_tests(AllTests)