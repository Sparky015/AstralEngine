
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
        Chessboard/Chessboard_PieceManagementTest.cpp
        Chessboard/InternalChessBoardDataTest.cpp
        ChessState/PieceTypeTests.cpp
        Chessboard/Chessboard_PieceMovement.cpp
)


add_executable(ChessTests
        ${TEST_FILES}
)

target_link_libraries(ChessTests
        GTest::gtest_main
        AstralEngine
        Chess)

add_dependencies(ChessTests Chess)

target_include_directories(ChessTests PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/googletest-main/googletest/include/)
target_include_directories(ChessTests PUBLIC ${CMAKE_SOURCE_DIR}/Source/Chess)

target_compile_options(ChessTests PRIVATE -Wno-unused-result)

include(GoogleTest)
gtest_discover_tests(ChessTests)