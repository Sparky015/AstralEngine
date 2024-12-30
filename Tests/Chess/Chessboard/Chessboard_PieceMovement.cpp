/**
* @file Chessboard_PieceMovement.cpp
* @author Andrew Fagan
* @date 12/30/2024
*/

#include <gtest/gtest.h>
#include "Chessboard/Chessboard.h"

class Chessboard_PieceMovementTest : public::testing::Test
{
public:
    struct TestCase
    {
        PieceID pieceID;
        PieceColor pieceColor;
        SquareLocation targetSquare;
        std::string_view testDescription;
    };
};

/**
 * @brief Verifies that move piece correctly modifies the state of the chessboard */
TEST_F(Chessboard_PieceMovementTest, MovePiece_ModifiesPieceDataCorrectly)
{
    const std::vector<TestCase> testCases = {
            { PIECE_1, PieceColor::BLACK, E3, "Black Piece 1 moves to E3"},
            { PIECE_2, PieceColor::WHITE, D4, "White Piece 2 moves to D4"},
            { PIECE_3, PieceColor::BLACK, F5, "Black Piece 3 moves to F5"},
            { PIECE_4, PieceColor::WHITE, H6, "White Piece 4 moves to H6"},
            { PIECE_6, PieceColor::WHITE, B4, "White Piece 6 moves to B4"},
            { PIECE_7, PieceColor::BLACK, C3, "Black Piece 7 moves to C3"},
    };

    for (const auto& [pieceID, pieceColor, targetSquare, testDescription]: testCases)
    {
        Game::Chessboard chessboard;
        chessboard.SetActiveColor(pieceColor);
        chessboard.MovePiece(pieceID, pieceColor, targetSquare);

        EXPECT_EQ(chessboard.GetPieceLocation(pieceID, pieceColor), targetSquare)
                            << "Location Comparison Failed. Test Case Description: " << testDescription;
    }
}


/**
 * @brief Verifies that move piece asserts fail on invalid input */
TEST_F(Chessboard_PieceMovementTest, MovePiece_AssertsFailOnInvalidInput)
{
    const std::vector<TestCase> testCases = {
            { PIECE_1, PieceColor::BLACK, SquareLocation(EMPTY), "Target location is not on the board" },
            { PIECE_1, PieceColor::BLACK, E7, "Target location already has a piece" },
            { PIECE_1, PieceColor::WHITE, E3, "Piece color is not active" }
    };


    for (const auto& [pieceID, pieceColor, targetSquare, testDescription]: testCases)
    {
        Game::Chessboard chessboard;
        chessboard.SetActiveColor(PieceColor::BLACK);

        EXPECT_ANY_THROW(chessboard.MovePiece(pieceID, pieceColor, targetSquare))
                            << "Expected a throw to occur. No throws happened. Test Failed. Test Case Description: " << testDescription;
    }
}

/**
 * @brief Verifies that move piece updates the half and full move counters accordingly */
TEST_F(Chessboard_PieceMovementTest, MovePiece_UpdatesHalfAndFullMoveCountersCorrectly)
{

}