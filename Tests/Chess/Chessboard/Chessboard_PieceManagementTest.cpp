/**
* @file Chessboard_PieceManagementTest.cpp
* @author Andrew Fagan
* @date 12/26/2024
*/

#include <gtest/gtest.h>
#include "Chessboard/Chessboard.h"

class Chessboard_PieceManagementTest : public::testing::Test
{
public:
    struct TestCase
    {
        PieceID pieceID;
        PieceType pieceType;
        PieceColor pieceColor;
        SquareLocation location;
        std::string_view testDescription;
    };
};

const std::vector<Chessboard_PieceManagementTest::TestCase> AddPieceTestCases{
        { PIECE_1, PieceType::PAWN, PieceColor::WHITE, SquareLocation(E3), "White Pawn added to E3" },
        { PIECE_2, PieceType::KNIGHT, PieceColor::BLACK, SquareLocation(D4), "Black Knight added to D4" },
        { PIECE_3, PieceType::BISHOP, PieceColor::WHITE, SquareLocation(F5), "White Bishop added to F5" },
        { PIECE_4, PieceType::ROOK, PieceColor::BLACK, SquareLocation(C6), "Black Rook added to C6" },
        { PIECE_5, PieceType::QUEEN, PieceColor::WHITE, SquareLocation(B2), "White Queen added to B2" },
        { PIECE_6, PieceType::KING, PieceColor::BLACK, SquareLocation(A7), "Black King added to A7" },
        { PIECE_7, PieceType::PAWN, PieceColor::BLACK, SquareLocation(G4), "Black Pawn added to G4" },
        { PIECE_8, PieceType::KNIGHT, PieceColor::WHITE, SquareLocation(H5), "White Knight added to H5" },
        { PIECE_9, PieceType::BISHOP, PieceColor::BLACK, SquareLocation(A3), "Black Bishop added to A3" },
        { PIECE_10, PieceType::ROOK, PieceColor::WHITE, SquareLocation(B1), "White Rook added to B1" },
        { PIECE_11, PieceType::QUEEN, PieceColor::BLACK, SquareLocation(C8), "Black Queen added to C8" },
        { PIECE_12, PieceType::KING, PieceColor::WHITE, SquareLocation(D2), "White King added to D2" },
        { PIECE_13, PieceType::PAWN, PieceColor::WHITE, SquareLocation(E4), "White Pawn added to E4" },
        { PIECE_14, PieceType::KNIGHT, PieceColor::BLACK, SquareLocation(F6), "Black Knight added to F6" },
        { PIECE_15, PieceType::BISHOP, PieceColor::WHITE, SquareLocation(G1), "White Bishop added to G1" },
        { PIECE_16, PieceType::ROOK, PieceColor::BLACK, SquareLocation(H8), "Black Rook added to H8" }
};

/**
 * @brief Verifies that the AddPiece function correctly adds a piece to the board
 * @note This test is important because AddPiece modifies the state when moves and captures occur
 */
TEST_F(Chessboard_PieceManagementTest, AddPiece_CorrectlyModifiesBoardState)
{
    for (const auto& [pieceID, pieceType, pieceColor, location, testDescription] : AddPieceTestCases)
    {
        Game::Chessboard chessboard;
        chessboard.ClearBoard();

        EXPECT_NO_THROW(chessboard.AddPiece(pieceID, pieceType, pieceColor, location))
            << "No throw check failed. Expected no throw. Throw occurred. Test Case Description: " << testDescription;

        EXPECT_EQ(chessboard.GetPieceLocation(pieceID, pieceColor), location)
            << "Final location comparison failed. Test Case Description: " << testDescription;


        // Checking if piece info was updated in all places

        EXPECT_EQ(chessboard.GetPieceType(pieceID, pieceColor), pieceType)
            << "PieceType (GetPieceType and given PieceType) comparison failed. Test Case Description: " << testDescription;

        EXPECT_EQ(chessboard.GetSquareType(location), pieceType)
            << "PieceType (GetSquareType and given PieceType) comparison failed. Test Case Description: " << testDescription;

        EXPECT_EQ(chessboard.GetSquareColor(location), pieceColor)
            << "Piece and square PieceColor comparison failed. Test Case Description: " << testDescription;

        EXPECT_EQ(chessboard.GetSquarePieceID(location), pieceID)
            << "Piece and square PieceID comparison failed. Test Case Description: " << testDescription;
    }
}

TEST_F(Chessboard_PieceManagementTest, AddPiece_AssertFailsWhenSquareIsOccuiped)
{
    Game::Chessboard chessboard; // Default board that has the starting positions

    // Checking if ASSERT fails when piece is already on board
    for (const auto& [pieceID, pieceType, pieceColor, location, testDescription] : AddPieceTestCases)
    {
        EXPECT_ANY_THROW(chessboard.AddPiece(pieceID, pieceType, pieceColor, location))
            << "No throw occurred. Test Failed. Test Case Description: " << testDescription;
    }
}


/**
 * @brief Verifies that the AddPiece ASSERTS fail on invalid input */
TEST_F(Chessboard_PieceManagementTest, AddPiece_AssertsFailOnInvalidLocations)
{
    Game::Chessboard chessboard;
    chessboard.ClearBoard();

    // Checking if ASSERT fails when location is invalid
    EXPECT_ANY_THROW(chessboard.AddPiece(PIECE_2, PieceType::PAWN, PieceColor::BLACK, SquareLocation(EMPTY)));
    EXPECT_ANY_THROW(chessboard.AddPiece(PIECE_2, PieceType::PAWN, PieceColor::BLACK, SquareLocation(64)));
    EXPECT_ANY_THROW(chessboard.AddPiece(PIECE_2, PieceType::PAWN, PieceColor::BLACK, SquareLocation(-1)));
    EXPECT_ANY_THROW(chessboard.AddPiece(PIECE_2, PieceType::PAWN, PieceColor::BLACK, SquareLocation(122)));
}

/**
 * @brief Verifies that the RemovePiece ASSERTS fail on invalid input */
TEST_F(Chessboard_PieceManagementTest, RemovePiece_AssertsFailOnInvalidInput)
{
    Game::Chessboard defaultChessboard; // Default board that has the starting positions
    Game::Chessboard emptyChessboard;
    emptyChessboard.ClearBoard();

    EXPECT_ANY_THROW(emptyChessboard.RemovePiece(PIECE_2, PieceColor::BLACK)); // Piece 2 already off board
    EXPECT_ANY_THROW(emptyChessboard.RemovePiece(PIECE_8, PieceColor::WHITE)); // Piece 8 already off board
    EXPECT_NO_THROW(defaultChessboard.RemovePiece(PIECE_2, PieceColor::BLACK)); // Piece 2 on board
    EXPECT_NO_THROW(defaultChessboard.RemovePiece(PIECE_8, PieceColor::WHITE)); // Piece 8 on board
}


//void RemovePiece(PieceID pieceID, PieceColor pieceColor);
/**
 * @brief Verifies that the RemovePiece correctly updates the piece's location as well as the state of the square that piece was on. */
TEST_F(Chessboard_PieceManagementTest, RemovePiece_BoardAndPieceStateUpdateCorrectly)
{
    struct TestCase
    {
        PieceID pieceID;
        PieceColor pieceColor;
        std::string_view testDescription;
    };

    std::vector<TestCase> testCases = {
        {PIECE_1, PieceColor::BLACK, "Remove Black Piece 1 from default position"},
        {PIECE_1, PieceColor::WHITE, "Remove White Piece 1 from default position"},
        {PIECE_2, PieceColor::WHITE, "Remove White Piece 2 from default position"},
        {PIECE_3, PieceColor::BLACK, "Remove Black Piece 3 from default position"},
        {PIECE_4, PieceColor::WHITE, "Remove White Piece 4 from default position"},
        {PIECE_5, PieceColor::BLACK, "Remove Black Piece 5 from default position"},
        {PIECE_6, PieceColor::WHITE, "Remove White Piece 6 from default position"},
        {PIECE_7, PieceColor::BLACK, "Remove Black Piece 7 from default position"},
        {PIECE_8, PieceColor::WHITE, "Remove White Piece 8 from default position"},
        {PIECE_16, PieceColor::BLACK, "Remove Black Piece 16 from default position"},
        {PIECE_16, PieceColor::WHITE, "Remove White Piece 16 from default position"},
    };


    for (const auto& [pieceID, pieceColor, testDescription] : testCases)
    {
        Game::Chessboard chessboard; // Default board that has the starting positions
        SquareLocation oldPieceLocation = chessboard.GetPieceLocation(pieceID, pieceColor);
        chessboard.RemovePiece(pieceID, pieceColor);

        EXPECT_EQ(chessboard.GetPieceLocation(pieceID, pieceColor), EMPTY);
        EXPECT_EQ(chessboard.GetSquareType(oldPieceLocation), PieceType::NONE);
    }
}
