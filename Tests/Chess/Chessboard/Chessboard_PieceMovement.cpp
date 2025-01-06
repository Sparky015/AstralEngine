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
    struct PieceMovementTestCase
    {
        PieceID pieceID;
        PieceColor pieceColor;
        SquareLocation targetSquare;
        std::string_view testDescription;
    };
};


std::vector<Chessboard_PieceMovementTest::PieceMovementTestCase> movePieceTestCases = {
    {PIECE_2, PieceColor::BLACK, D5, "Black Piece 2 moves to D5"},
    {PIECE_2, PieceColor::WHITE, D5, "White Piece 2 moves to D5"},
    {PIECE_3, PieceColor::BLACK, C3, "Black Piece 3 moves to C6"},
    {PIECE_3, PieceColor::WHITE, C3, "White Piece 3 moves to C6"},
    {PIECE_8, PieceColor::BLACK, B6, "Black Piece 8 moves to B7"},
    {PIECE_8, PieceColor::WHITE, B6, "White Piece 8 moves to B7"},
    {PIECE_16, PieceColor::BLACK, A5, "Black Piece 16 moves to A8"},
    {PIECE_16, PieceColor::WHITE, A5, "White Piece 16 moves to A8"},
};


/**
 * @brief Verifies that move piece correctly modifies the state of the chessboard */
TEST_F(Chessboard_PieceMovementTest, MovePiece_ModifiesPieceDataCorrectly)
{
    for (const auto& [pieceID, pieceColor, targetSquare, testDescription]: movePieceTestCases)
    {
        Game::Chessboard chessboard;
        chessboard.SetActiveColor(pieceColor);
        SquareLocation oldPieceLocation = chessboard.GetPieceLocation(pieceID, pieceColor);
        PieceType pieceType = chessboard.GetPieceType(pieceID, pieceColor);
        EXPECT_NO_THROW(chessboard.MovePiece(pieceID, pieceColor, targetSquare));

        EXPECT_EQ(chessboard.GetPieceLocation(pieceID, pieceColor), targetSquare)
                            << "Piece location updated check failed. Test Case Description: " << testDescription;

        EXPECT_EQ(chessboard.GetSquareType(targetSquare), pieceType)
                                    << "New square type is the piece's type check failed. Test Case Description: " << testDescription;

        EXPECT_EQ(chessboard.GetSquareType(oldPieceLocation), PieceType::NONE)
                            << "Old square type is PieceType::NONE check failed. Test Case Description: " << testDescription;
    }
}


/**
 * @brief Verifies that move piece asserts fail on invalid input */
TEST_F(Chessboard_PieceMovementTest, MovePiece_AssertsFailOnInvalidInput)
{
    const std::vector<PieceMovementTestCase> testCases = {
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
 * @brief Verifies that move piece updates the half and full move counters accordingly on non-pawn moves */
TEST_F(Chessboard_PieceMovementTest, MovePiece_UpdatesHalfAndFullMoveCountersCorrectly)
{
    for (const auto& [pieceID, pieceColor, targetSquare, testDescription] : movePieceTestCases)
    {
        Game::Chessboard chessboard;
        const uint8 initFullMoveCount = chessboard.GetFullMoveCount();
        const uint8 initHalfMoveCount = chessboard.GetHalfMoveCount();


        chessboard.SetActiveColor(pieceColor);
        chessboard.SetPieceType(pieceID, PieceType::ROOK, pieceColor);
        chessboard.MovePiece(pieceID, pieceColor, targetSquare);
        EXPECT_EQ(initHalfMoveCount + 1, chessboard.GetHalfMoveCount())
            << "Half move comparison failed! Test Case Description: " << testDescription;
        if (pieceColor.IsBlack())
        {
            EXPECT_EQ(initFullMoveCount + 1, chessboard.GetFullMoveCount())
                << "Full move comparison failed! Expected full move to increment. Test Case Description: " << testDescription;
        }
        else
        {
            EXPECT_EQ(initFullMoveCount, chessboard.GetFullMoveCount())
                << "Full move comparison failed! Expected full move to not increment. Test Case Description: " << testDescription;
        }
    }

}


/**
 * @brief Verifies that move piece updates the half and full move counters accordingly on non-pawn moves */
TEST_F(Chessboard_PieceMovementTest, MovePiece_ResetsHalfAndFullMoveCountersOnPawnMoves)
{
    for (const auto& [pieceID, pieceColor, targetSquare, testDescription] : movePieceTestCases)
    {
        Game::Chessboard chessboard;
        const uint8 initFullMoveCount = chessboard.GetFullMoveCount();

        chessboard.SetActiveColor(pieceColor);
        chessboard.SetPieceType(pieceID, PieceType::PAWN, pieceColor);
        chessboard.MovePiece(pieceID, pieceColor, targetSquare);
        EXPECT_EQ(chessboard.GetHalfMoveCount(), 1) // Half move count should reset to 1 on pawn moves
            << "Half move comparison failed! Test Case Description: " << testDescription;

        if (pieceColor.IsBlack())
        {
            EXPECT_EQ(initFullMoveCount + 1, chessboard.GetFullMoveCount())
                << "Full move comparison failed! Expected full move to increment. Test Case Description: " << testDescription;
        }
        else
        {
            EXPECT_EQ(initFullMoveCount, chessboard.GetFullMoveCount())
                << "Full move comparison failed! Expected full move to not increment. Test Case Description: " << testDescription;
        }
    }
}


/**@brief Verifies that capture piece ASSERTS will fail on invalid input */
TEST_F(Chessboard_PieceMovementTest, CapturePiece_AssertsFailOnInvalidInput)
{
    const std::vector<PieceMovementTestCase> testCases = {
        { PIECE_2, PieceColor::BLACK, SquareLocation(EMPTY), "Capturing on a square not on the board" },
        { PIECE_1, PieceColor::BLACK, E5, "Capturing on square with no piece" },
        { PIECE_1, PieceColor::WHITE, E8, "Capturing when turn is not active" },
        { PIECE_1, PieceColor::BLACK, A8, "Capturing piece of same color" },
    };

    for (const auto& [pieceID, pieceColor, targetSquare, testDescription] : testCases)
    {
        Game::Chessboard chessboard;

        chessboard.SetActiveColor(PieceColor::BLACK);
        EXPECT_ANY_THROW(chessboard.CapturePiece(pieceID, pieceColor, targetSquare))
            << "Assert did not fail. Expected to fail. Test Case Description: " << testDescription;
    }

    Game::Chessboard chessboard;
    chessboard.SetPieceType(PIECE_1, PieceType::NONE, PieceColor::WHITE);
    EXPECT_ANY_THROW(chessboard.CapturePiece(PIECE_1, PieceColor::WHITE, E4))
        << "Assert did not fail. Expected to fail. Test Case Description: Capturing with a piece of type NONE";
}


const std::vector<Chessboard_PieceMovementTest::PieceMovementTestCase> capturePieceTestCases = {
    { PIECE_2, PieceColor::BLACK, E2, "Black Piece 2 is capturing white piece on E2" },
    { PIECE_12, PieceColor::BLACK, A1,  "Black Piece 12 is capturing white piece on A1"},
    { PIECE_16, PieceColor::WHITE, A8,  "White Piece 16 is capturing white piece on A8"},
    { PIECE_1, PieceColor::WHITE, G7, "White Piece 1 is capturing white piece on G7"},
    { PIECE_4, PieceColor::BLACK, B2, "Black Piece 4 is capturing white piece on B2" },
    { PIECE_6, PieceColor::BLACK, C1, "Black Piece 6 is capturing white piece on C1" },
    { PIECE_10, PieceColor::WHITE, H8, "White Piece 10 is capturing black piece on H8" },
    { PIECE_14, PieceColor::WHITE, F7, "White Piece 14 is capturing black piece on F7" },
};


/**@brief Verifies that capture piece ASSERTS will fail on invalid input */
TEST_F(Chessboard_PieceMovementTest, CapturePiece_ModifiesPieceDataCorrectly)
{


    for (const auto& [pieceID, pieceColor, targetSquare, testDescription] : capturePieceTestCases)
    {
        Game::Chessboard chessboard;
        const SquareLocation oldPieceLocation = chessboard.GetPieceLocation(pieceID, pieceColor);
        const PieceType pieceType = chessboard.GetPieceType(pieceID, pieceColor);
        const PieceID targetPieceID = chessboard.GetSquarePieceID(targetSquare);
        const PieceColor targetPieceColor = pieceColor.Opposite();

        chessboard.SetActiveColor(pieceColor);
        EXPECT_NO_THROW(chessboard.CapturePiece(pieceID, pieceColor, targetSquare));

        EXPECT_EQ(chessboard.GetPieceLocation(pieceID, pieceColor), targetSquare)
                << "Piece location updated check failed. Test Case Description: " << testDescription;

        EXPECT_EQ(chessboard.GetSquareType(targetSquare), pieceType)
                << "New square type is the piece's type check failed. Test Case Description: " << testDescription;

        EXPECT_EQ(chessboard.GetSquareType(oldPieceLocation), PieceType::NONE)
                << "Old square type is PieceType::NONE check failed. Test Case Description: " << testDescription;

        EXPECT_EQ(chessboard.GetPieceLocation(targetPieceID, targetPieceColor), EMPTY)
                << "Target piece location updated to EMPTY check failed. Test Case Description: " << testDescription;
    }

}



/**
 * @brief Verifies that move piece updates the half and full move counters accordingly on non-pawn moves */
TEST_F(Chessboard_PieceMovementTest, CapturePiece_ChangesHalfAndFullMoveCountersCorrectly)
{
    for (const auto& [pieceID, pieceColor, targetSquare, testDescription] : capturePieceTestCases)
    {
        Game::Chessboard chessboard;
        const uint8 initFullMoveCount = chessboard.GetFullMoveCount();
        const uint8 initHalfMoveCount = chessboard.GetHalfMoveCount();

        chessboard.SetActiveColor(pieceColor);
        chessboard.SetPieceType(pieceID, PieceType::PAWN, pieceColor);
        chessboard.CapturePiece(pieceID, pieceColor, targetSquare);
        EXPECT_EQ(chessboard.GetHalfMoveCount(), 1) // Half move count should reset on every capture move
            << "Half move comparison failed! Test Case Description: " << testDescription;

        if (pieceColor.IsBlack())
        {
            EXPECT_EQ(initFullMoveCount + 1, chessboard.GetFullMoveCount())
                << "Full move comparison failed! Expected full move to increment. Test Case Description: " << testDescription;
        }
        else
        {
            EXPECT_EQ(initFullMoveCount, chessboard.GetFullMoveCount())
                << "Full move comparison failed! Expected full move to not increment. Test Case Description: " << testDescription;
        }
    }
}