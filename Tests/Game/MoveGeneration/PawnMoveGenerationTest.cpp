//
// Created by Andrew Fagan on 11/24/24.
//

#include <gtest/gtest.h>
#include "Game/Board/Board.h"
#include "Game/MoveList.h"
#include "Game/Board/Conversions.h"

namespace Game {

    class PawnMoveGenerationTest : public ::testing::Test
    {
    public:
        void SetUp() override
        {
            whiteMoveList.GenerateMoves(FEN_board, PieceColor::WHITE);
            blackMoveList.GenerateMoves(FEN_board, PieceColor::BLACK);
        }

        Board standardBoard{Board()};
        Board FEN_board{Board("rnbqk2r/p1p3p1/5n2/Pp1ppp1p/1b1P1PP1/2P1P3/1P1B3P/RN1QKBNR w KQkq b6 0 9")};
        MoveList whiteMoveList{standardBoard, PieceColor::WHITE};
        MoveList blackMoveList{standardBoard, PieceColor::BLACK};
    };

    TEST_F(PawnMoveGenerationTest, RegularMove)
    {
        PieceID pieceID = FEN_board.ReadSquarePieceID(A5);
        std::vector<uint8> regularMoves = whiteMoveList.GetRegularMoves(pieceID);
        LOG(VectorToString(regularMoves));

        PieceID pieceID2 = FEN_board.ReadSquarePieceID(B2);
        std::vector<uint8> regularMoves2 = whiteMoveList.GetRegularMoves(pieceID2);
        LOG(VectorToString(regularMoves2));

        PieceID pieceID3 = FEN_board.ReadSquarePieceID(G4);
        std::vector<uint8> regularMoves3 = whiteMoveList.GetRegularMoves(pieceID3);
        LOG(VectorToString(regularMoves3));
    }

}