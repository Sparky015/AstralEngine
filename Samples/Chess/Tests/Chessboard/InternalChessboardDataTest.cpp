/**
* @file InternalChessBoardDataTest.cpp
* @author Andrew Fagan
* @date 12/26/2024
*/

#include <gtest/gtest.h>
#include "Chessboard/Chessboard.h"

using namespace Game;

/**@brief Tests for the InternalChessBoard class
 * @note The chessboard is the starting position with piece data populated. */
class InternalChessboardDataTest : public ::testing::Test
{
public:

    void SetUp() override
    {
        board = chessboard.GetInternalBoard();
    }

    Chessboard chessboard;
    InternalBoardRepresentation board = InternalBoardRepresentation();
};

/**@brief Verifies that ReadSquareColor correctly returns the correct color */
TEST_F(InternalChessboardDataTest, ReadSquareColor_ReturnTheCorrectCorrespondingColor)
{
    EXPECT_EQ(board.ReadSquareColor(C8), PieceColor::BLACK);
    EXPECT_EQ(board.ReadSquareColor(B1), PieceColor::WHITE);
    EXPECT_EQ(board.ReadSquareColor(A7), PieceColor::BLACK);
    EXPECT_EQ(board.ReadSquareColor(D1), PieceColor::WHITE);
    EXPECT_EQ(board.ReadSquareColor(F7), PieceColor::BLACK);
    EXPECT_EQ(board.ReadSquareColor(G2), PieceColor::WHITE);
    EXPECT_EQ(board.ReadSquareColor(H5), PieceColor::BLACK);
}

/**@brief Verifies that WriteSquareColor correctly serializing the piece color into the uint8 */
TEST_F(InternalChessboardDataTest, WriteSquareColor_CorrectlySerializesSquareColor)
{
    board.WriteSquareColor(PieceColor::WHITE, F3);
    EXPECT_EQ(board.ReadSquareColor(F3), PieceColor::WHITE);

    board.WriteSquareColor(PieceColor::BLACK, G8);
    EXPECT_EQ(board.ReadSquareColor(G8), PieceColor::BLACK);

    board.WriteSquareColor(PieceColor::WHITE, G8);
    EXPECT_EQ(board.ReadSquareColor(G8), PieceColor::WHITE);
}

/**@brief Verifies that ReadSquareType correctly returns the correct type */
TEST_F(InternalChessboardDataTest, TypeRead1)
{
    EXPECT_EQ(board.ReadSquareType(F7), PieceType::PAWN);
    EXPECT_EQ(board.ReadSquareType(G2), PieceType::PAWN);
    EXPECT_EQ(board.ReadSquareType(E4), PieceType::NONE);
    EXPECT_EQ(board.ReadSquareType(B1), PieceType::KNIGHT);
}

/**@brief Verifies that WriteSquareType is correctly serializing the piece type into the uint8 */
TEST_F(InternalChessboardDataTest, TypeWrite1)
{
    board.WriteSquareType(PieceType::PAWN, E4);
    EXPECT_EQ(board.ReadSquareType(E4), PieceType::PAWN);
    EXPECT_EQ(board.ReadSquareType(D4), PieceType::NONE);
    EXPECT_EQ(board.ReadSquareType(F4), PieceType::NONE);

    board.WriteSquareType(PieceType::QUEEN, F3);
    EXPECT_EQ(board.ReadSquareType(F3), PieceType::QUEEN);
    EXPECT_EQ(board.ReadSquareType(E3), PieceType::NONE);
    EXPECT_EQ(board.ReadSquareType(G3), PieceType::NONE);

    board.WriteSquareType(PieceType::ROOK, C3);
    board.WriteSquareType(PieceType::KING, D3);
    board.WriteSquareType(PieceType::QUEEN, E3);
    EXPECT_EQ(board.ReadSquareType(C3), PieceType::ROOK);
    EXPECT_EQ(board.ReadSquareType(D3), PieceType::KING);
    EXPECT_EQ(board.ReadSquareType(E3), PieceType::QUEEN);
}