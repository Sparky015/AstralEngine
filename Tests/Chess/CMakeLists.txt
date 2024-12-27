
#Enabling the tests for CMake
enable_testing()

set(TEST_FILES
        Game/Board/BoardClass/Board_MovePieceTest.cpp
        Game/Board/BoardClass/Board_FEN_Constructor.cpp
        Game/Board/BoardClass/Board_Standard_Constructor.cpp
        Game/PieceTest.cpp
        Game/Board/BoardClass/Board_InternalBoardRepresentation.cpp
        Game/Board/BoardClass/Board_PromotePawn.cpp
        Game/Board/BoardClass/Board_TakePiece.cpp
        Game/Board/BoardClass/Board_ReadSquarePieceID.cpp
        Game/Board/BoardClass/Board_GetCharacterOfPiece.cpp
        Game/ConversionsTest.cpp
        Game/Pieces/PawnMoveGenerationTest.cpp
        Game/MoveGenerationHelperTest.cpp
        Game/Pieces/RookMoveGenerationTest.cpp
        ChessState/SquareLocationTest.cpp
        ChessBoard/ChessBoardTest.cpp
        ChessBoard/InternalChessBoardDataTest.cpp
)


add_executable(ChessTests
        ${TEST_FILES}
)

target_link_libraries(ChessTests GTest::gtest_main)
target_link_libraries(ChessTests SozinEngine)

target_include_directories(ChessTests PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/googletest-main/googletest/include/)
target_include_directories(ChessTests PUBLIC ${CMAKE_SOURCE_DIR}/Source/Chess)

include(GoogleTest)
gtest_discover_tests(ChessTests)