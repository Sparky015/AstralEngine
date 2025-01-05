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
        {PIECE_1, PieceColor::BLACK, "Remove Black Piece 1"},
        {PIECE_1, PieceColor::WHITE, "Remove White Piece 1"},
        {PIECE_2, PieceColor::WHITE, "Remove White Piece 2"},
        {PIECE_3, PieceColor::BLACK, "Remove Black Piece 3"},
        {PIECE_4, PieceColor::WHITE, "Remove White Piece 4"},
        {PIECE_5, PieceColor::BLACK, "Remove Black Piece 5"},
        {PIECE_6, PieceColor::WHITE, "Remove White Piece 6"},
        {PIECE_7, PieceColor::BLACK, "Remove Black Piece 7"},
        {PIECE_8, PieceColor::WHITE, "Remove White Piece 8"},
        {PIECE_16, PieceColor::BLACK, "Remove Black Piece 16"},
        {PIECE_16, PieceColor::WHITE, "Remove White Piece 16"},
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


TEST_F(Chessboard_PieceManagementTest, PromotePawn_ValidPawnPromotions) {

    Game::Chessboard board;

    struct TestCase
    {
        PieceID pieceID;
        PieceColor pieceColor;
        PieceType promotionType;
    };


    std::vector<TestCase> testCases = {
        {PIECE_1, PieceColor::WHITE, PieceType::BISHOP},
        {PIECE_1, PieceColor::WHITE, PieceType::KNIGHT},
        {PIECE_1, PieceColor::WHITE, PieceType::ROOK},
        {PIECE_1, PieceColor::WHITE, PieceType::QUEEN},
        {PIECE_5, PieceColor::WHITE, PieceType::ROOK},
        {PIECE_16, PieceColor::WHITE, PieceType::QUEEN},
    };

    for (const auto& [pieceID, pieceColor, promotionType] : testCases)
    {
        board.SetPieceType(pieceID, PieceType::PAWN, pieceColor); // Set the piece to a pawn

        EXPECT_NO_THROW(board.PromotePawn(pieceID, pieceColor, promotionType));
        EXPECT_EQ(board.GetPieceType(pieceID, pieceColor), promotionType);
    }

}

TEST_F(Chessboard_PieceManagementTest, PromotePawn_InvalidPromotionTypes)
{
    Game::Chessboard chessboard;

    chessboard.SetPieceType(PIECE_1, PieceType::PAWN, PieceColor::WHITE);

    // Invalid promotion to PAWN
    EXPECT_ANY_THROW(chessboard.PromotePawn(PIECE_1, PieceColor::WHITE, PieceType::PAWN)) <<
                 "Promotion type must be bishop, knight, rook, or queen.";

    // Invalid promotion to KING
    EXPECT_ANY_THROW(chessboard.PromotePawn(PIECE_1, PieceColor::WHITE, PieceType::KING)) <<
                 "Promotion type must be bishop, knight, rook, or queen.";
}

TEST_F(Chessboard_PieceManagementTest, PromotePawn_CannotPromoteNonPawns)
{
    Game::Chessboard board;
    board.SetPieceType(PIECE_1, PieceType::ROOK, PieceColor::WHITE);

    // Promoting a rook is not allowed. Should throw.
    EXPECT_ANY_THROW(board.PromotePawn(PIECE_1, PieceColor::WHITE, PieceType::QUEEN)) <<
                 "Piece must be a pawn to promote it.";
}



TEST_F(Chessboard_PieceManagementTest, ClearBoard_ResetsAllSquaresToEmpty)
{
    Game::Chessboard chessboard; // Default board with starting positions

    // Clear the board
    EXPECT_NO_THROW(chessboard.ClearBoard())
        << "ClearBoard should not throw any exceptions";

    // Verify all squares are empty by checking each square from A1 to H8
    for (SquareLocation square = 0; square < 64; ++square)
    {
        EXPECT_EQ(chessboard.GetSquareType(square), PieceType::NONE)
            << "Square " << square.GetString() << " should be empty after board clear";
    }
}

TEST_F(Chessboard_PieceManagementTest, ClearBoard_ResetsAllPieceLocations)
{
    Game::Chessboard chessboard; // Default board with starting positions

    // Clear the board
    chessboard.ClearBoard();

    // Test all piece locations for both colors
    for (uint8 pieceID = PIECE_1; pieceID <= PIECE_16; ++pieceID)
    {
        EXPECT_EQ(chessboard.GetPieceLocation((PieceID)pieceID, PieceColor::WHITE), EMPTY)
            << "White piece " << pieceID << " location should be EMPTY after board clear";

        EXPECT_EQ(chessboard.GetPieceLocation((PieceID)pieceID, PieceColor::BLACK), EMPTY)
            << "Black piece " << pieceID << " location should be EMPTY after board clear";
    }
}

TEST_F(Chessboard_PieceManagementTest, ClearBoard_CanAddPiecesAfterClearing)
{
    Game::Chessboard chessboard;
    chessboard.ClearBoard();

    // Test adding pieces after clearing using the same test cases from AddPieceTestCases
    for (const auto& [pieceID, pieceType, pieceColor, location, testDescription] : AddPieceTestCases)
    {
        EXPECT_NO_THROW(chessboard.AddPiece(pieceID, pieceType, pieceColor, location))
            << "Failed to add piece after board clear. Test Case Description: " << testDescription;

        EXPECT_EQ(chessboard.GetPieceLocation(pieceID, pieceColor), location)
            << "Piece location mismatch after adding to cleared board. Test Case Description: " << testDescription;

        EXPECT_EQ(chessboard.GetPieceType(pieceID, pieceColor), pieceType)
            << "Piece type mismatch after adding to cleared board. Test Case Description: " << testDescription;
    }
}

TEST_F(Chessboard_PieceManagementTest, ClearBoard_MultipleClears)
{
    Game::Chessboard chessboard;

    // Clear multiple times to ensure it's idempotent
    for (int i = 0; i < 3; ++i)
    {
        EXPECT_NO_THROW(chessboard.ClearBoard())
            << "Multiple ClearBoard calls should not throw exceptions";

        // Verify a sample square and piece after each clear
        EXPECT_EQ(chessboard.GetSquareType(E4), PieceType::NONE)
            << "Square E4 should be empty after clear attempt " << i + 1;

        EXPECT_EQ(chessboard.GetPieceLocation(PIECE_1, PieceColor::WHITE), EMPTY)
            << "Piece location should be EMPTY after clear attempt " << i + 1;
    }
}