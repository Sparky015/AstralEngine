

#Enabling the tests for CMake
enable_testing()

set(TEST_FILES
        ExampleTest.cpp
        Game/Board/BoardClass/Board_MovePieceTest.cpp
        Game/Board/BoardClass/Board_FEN_Constructor.cpp
        Core/Events/EventSystemTest.cpp
        Game/Board/BoardClass/Board_Standard_Constructor.cpp
        Game/PieceTest.cpp
        Game/Board/BoardClass/Board_InternalBoardRepresentation.cpp
        Game/Board/BoardClass/Board_PromotePawn.cpp
        Game/Board/BoardClass/Board_TakePiece.cpp
        Game/Board/BoardClass/Board_ReadSquarePieceID.cpp
        Game/Board/BoardClass/Board_GetCharacterOfPiece.cpp
        Game/Board/ConversionsTest.cpp
        Game/Pieces/PawnMoveGenerationTest.cpp
        Game/MoveGenerationHelperTest.cpp
        Game/Pieces/RookMoveGenerationTest.cpp
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