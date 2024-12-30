/**
* @file ChessBoardTest.cpp
* @author Andrew Fagan
* @date 12/26/2024
*/

#include <gtest/gtest.h>

#include "Chessboard/Chessboard.h"

class ChessBoardTest : public::testing::Test
{
public:

};

/**
 * @brief Verifies that move piece correctly modifies the state of the chessboard
 * @test Ensures that */
TEST_F(ChessBoardTest, MovePiece_ModifiesBoardStateCorrectly)
{
    struct TestCase
    {
        PieceID pieceID;
        PieceColor pieceColor;
        SquareLocation targetSquare;
        bool expectedResult;
        std::string_view testDescription;
    };

    const std::vector<TestCase> testCases = {
        { PIECE_1, PieceColor::BLACK, E3, true, "Black Piece 1 moves to E3"},
        { PIECE_2, PieceColor::WHITE, D4, true, "White Piece 2 moves to D4"},
        { PIECE_3, PieceColor::BLACK, F5, false, "Black Piece 3 fails to move to F5"},
        { PIECE_4, PieceColor::WHITE, H6, true, "White Piece 4 moves to H6"},
        { PIECE_5, PieceColor::BLACK, A1, false, "Black Piece 5 fails to move to A1"},
        { PIECE_6, PieceColor::WHITE, B2, true, "White Piece 6 moves to B2"},
        { PIECE_7, PieceColor::BLACK, C3, true, "Black Piece 7 moves to C3"},
        { PIECE_8, PieceColor::WHITE, G7, false, "White Piece 8 fails to move to G7"}
    };

    for (const auto& [pieceID, pieceColor, targetSquare, expectedResult, testDescription]: testCases)
    {
        Game::Chessboard chessboard;
        chessboard.SetActiveColor(pieceColor);
        SquareLocation oldPieceLocation = chessboard.GetPieceLocation(pieceID, pieceColor);
        chessboard.MovePiece(pieceID, pieceColor, targetSquare);

        EXPECT_EQ(chessboard.GetPieceLocation(pieceID, pieceColor) == targetSquare, expectedResult)
            << "Comparison Failed. Test Case Description: " << testDescription;

        EXPECT_EQ(chessboard.GetPieceLocation(pieceID, pieceColor) != oldPieceLocation, expectedResult)
            << "Comparison Failed. Test Case Description: " << testDescription;
    }
}



/**
 * @brief Verifies that move piece asserts fail on invalid input
 * @test Ensures that asserts behave correctly on valid and invalid input */
TEST_F(ChessBoardTest, MovePiece_AssertsFailOnInvalidInput)
{
    struct TestCase
    {
        PieceID pieceID;
        PieceColor pieceColor;
        SquareLocation targetSquare;
        bool expectedResult;
        std::string_view testDescription;
    };

    const std::vector<TestCase> testCases = {
        { PIECE_1, PieceColor::BLACK, SquareLocation(EMPTY), true, "Target location is not on the board" },
        { PIECE_1, PieceColor::BLACK, E7, true, "Target location already has a piece" },
        { PIECE_1, PieceColor::WHITE, E3, true, "Piece color is not active" }
    };


    for (const auto& [pieceID, pieceColor, targetSquare, expectedResult, testDescription]: testCases)
    {
        Game::Chessboard chessboard;
        chessboard.SetActiveColor(PieceColor::BLACK);

        if (expectedResult)
        {
            EXPECT_ANY_THROW(chessboard.MovePiece(pieceID, pieceColor, targetSquare))
                << "Expected a throw to occur. No throws happened. Test Failed. Test Case Description: " << testDescription;
        }
        else
        {
            EXPECT_NO_THROW(chessboard.MovePiece(pieceID, pieceColor, targetSquare))
                << "Expected no throw to occur. A throw happened. Test Failed. Test Case Description: " << testDescription;
        }
    }

    //
}

//void Chessboard::AddPiece(PieceID pieceID, PieceType pieceType, PieceColor pieceColor, SquareLocation location)
/**
 * @brief Verifies that the AddPiece function correctly adds a piece to the board
 * @test Ensures that the AddPiecee correctly modifies the state of the chessboard
 * @note This test is important because AddPiece modifies the state when moves and captures occur
 */
TEST_F(ChessBoardTest, AddPiece_CorrectlyModifiesBoardState)
{
    struct TestCase
    {
        PieceID pieceID;
        PieceType pieceType;
        PieceColor pieceColor;
        SquareLocation location;
        bool expectedResult;
        std::string_view testDescription;
    };

    const std::vector<TestCase> testCases{
    { PIECE_1, PieceType::PAWN, PieceColor::WHITE, SquareLocation(E3), true, "White Pawn added to E3" },
    { PIECE_2, PieceType::KNIGHT, PieceColor::BLACK, SquareLocation(D4), true, "Black Knight added to D4" },
    { PIECE_3, PieceType::BISHOP, PieceColor::WHITE, SquareLocation(F5), true, "White Bishop added to F5" },
    { PIECE_4, PieceType::ROOK, PieceColor::BLACK, SquareLocation(C6), true, "Black Rook added to C6" },
    { PIECE_5, PieceType::QUEEN, PieceColor::WHITE, SquareLocation(B2), true, "White Queen added to B2" },
    { PIECE_6, PieceType::KING, PieceColor::BLACK, SquareLocation(A7), true, "Black King added to A7" },
    { PIECE_7, PieceType::PAWN, PieceColor::BLACK, SquareLocation(G4), true, "Black Pawn added to G4" },
    { PIECE_8, PieceType::KNIGHT, PieceColor::WHITE, SquareLocation(H5), true, "White Knight added to H5" },
    { PIECE_9, PieceType::BISHOP, PieceColor::BLACK, SquareLocation(A3), true, "Black Bishop added to A3" },
    { PIECE_10, PieceType::ROOK, PieceColor::WHITE, SquareLocation(B1), true, "White Rook added to B1" },
    { PIECE_11, PieceType::QUEEN, PieceColor::BLACK, SquareLocation(C8), true, "Black Queen added to C8" },
    { PIECE_12, PieceType::KING, PieceColor::WHITE, SquareLocation(D2), true, "White King added to D2" },
    { PIECE_13, PieceType::PAWN, PieceColor::WHITE, SquareLocation(E4), true, "White Pawn added to E4" },
    { PIECE_14, PieceType::KNIGHT, PieceColor::BLACK, SquareLocation(F6), true, "Black Knight added to F6" },
    { PIECE_15, PieceType::BISHOP, PieceColor::WHITE, SquareLocation(G1), true, "White Bishop added to G1" },
    { PIECE_16, PieceType::ROOK, PieceColor::BLACK, SquareLocation(H8), true, "Black Rook added to H8" }
    };

    for (const auto& [pieceID, pieceType, pieceColor, location ,expectedResult, testDescription] : testCases)
    {
        Game::Chessboard chessboard;
        chessboard.ClearBoard();

        EXPECT_NO_THROW(chessboard.AddPiece(pieceID, pieceType, pieceColor, location));

        EXPECT_EQ(chessboard.GetPieceLocation(pieceID, pieceColor) == location, expectedResult)
            << "Final location comparison failed. Test Case Description: " << testDescription;

        EXPECT_EQ(chessboard.GetPieceType(pieceID, pieceColor) == pieceType, expectedResult)
            << "PieceType (GetPieceType and given PieceType) comparison failed. Test Case Description: " << testDescription;

        EXPECT_EQ(chessboard.GetSquareType(location) == pieceType, expectedResult)
            << "PieceType (GetSquareType and given PieceType) comparison failed. Test Case Description: " << testDescription;

        EXPECT_EQ(chessboard.GetSquareColor(location) == pieceColor, expectedResult)
            << "Piece and square PieceColor comparison failed. Test Case Description: " << testDescription;

        EXPECT_EQ(chessboard.GetSquarePieceID(location) == pieceID, expectedResult)
            << "Piece and square PieceID comparison failed. Test Case Description: " << testDescription;
    }


    // Ensure the ASSERTS are failing on invalid input

    {
        Game::Chessboard chessboard; // Default board that has the starting positions

        // Checking if ASSERT fails when piece is already on board
        for (const auto& [pieceID, pieceType, pieceColor, location ,expectedResult, testDescription] : testCases)
        {
            EXPECT_ANY_THROW(chessboard.AddPiece(pieceID, pieceType, pieceColor, location))
                << "No throw occurred. Test Failed. Test Case Description: " << testDescription;
        }

        chessboard.ClearBoard();

        // Checking if ASSERT fails when location is invalid
        EXPECT_ANY_THROW(chessboard.AddPiece(PIECE_2, PieceType::PAWN, PieceColor::BLACK, SquareLocation(EMPTY)));
        EXPECT_ANY_THROW(chessboard.AddPiece(PIECE_2, PieceType::PAWN, PieceColor::BLACK, SquareLocation(64)));
        EXPECT_ANY_THROW(chessboard.AddPiece(PIECE_2, PieceType::PAWN, PieceColor::BLACK, SquareLocation(-1)));
        EXPECT_ANY_THROW(chessboard.AddPiece(PIECE_2, PieceType::PAWN, PieceColor::BLACK, SquareLocation(122)));
    }
}