/**
* @file Board.cpp
* @author Andrew Fagan
* @date 10/20/2024
*/

#include "Board.h"


Board::Board(const bool isDefaultBoard)
{

}


Board::Board(const std::string& FEN)
{

}


void Board::MovePiece(const PieceColor color, const PieceID pieceID, const uint8_t targetBoardLocation)
{

}


void Board::TakePiece(const PieceColor color, const PieceID pieceID, const uint8_t targetBoardLocation)
{

}


void Board::PromotePawn(const PieceColor color, const PieceID pieceIndex, const PieceType promotionType) const
{

}


uint8 Board::ReadBoardLocation(const PieceColor color, const PieceID pieceID) const
{
    return 0;
}


void Board::WriteBoardLocation(const PieceColor color, const PieceID pieceID, const uint8_t boardLocation)
{

}



