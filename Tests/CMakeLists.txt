

#Enabling the tests for CMake
enable_testing()

set(TEST_FILES
        ExampleTest.cpp
        Game/BoardClass/Board_MovePieceTest.cpp
        Game/BoardClass/Board_FEN_Constructor.cpp
        Core/Events/EventSystemTest.cpp
        Game/BoardClass/Board_Standard_Constructor.cpp
        Game/PieceTest.cpp
        Game/BoardClass/Board_InternalBoardRepresentation.cpp
        Game/BoardClass/Board_PromotePawn.cpp
        Game/BoardClass/Board_TakePiece.cpp
        Game/BoardClass/Board_ReadSquarePieceID.cpp
        Game/BoardClass/Board_GetCharacterOfPiece.cpp
        Game/ConversionsTest.cpp
        Game/MoveGeneration/PawnMoveGenerationTest.cpp
        Game/MoveGeneration/BoundsCheckerTest.cpp
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