

#Enabling the tests for CMake
enable_testing()

set(TEST_FILES
        ExampleTest.cpp
        "Game/BoardClass/Board_MovePieceTest.cpp"
        Game/BoardClass/Board_FEN_Constructor.cpp
        Core/Events/EventSystemTest.cpp
)

list(TRANSFORM SOURCE_FILES PREPEND ${CMAKE_SOURCE_DIR}/Source/) # Set up the SOURCE_FILES variable for use in this CMakeList file

add_executable(
        AllTests
        ${TEST_FILES}
        ${SOURCE_FILES}
)

target_link_libraries(
        AllTests
        GTest::gtest_main
)

target_include_directories(AllTests PRIVATE ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/googletest-main/googletest/include/)
target_include_directories(AllTests PRIVATE ${CMAKE_SOURCE_DIR}/Source)

include(GoogleTest)
gtest_discover_tests(AllTests)