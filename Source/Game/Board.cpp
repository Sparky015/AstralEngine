/**
* @file Board.cpp
* @author Andrew Fagan
* @date 10/20/2024
*/

#include "Board.h"


Board::Board(const bool isDefaultBoard) :
        m_BlackPieceTypes({PieceType::PAWN,PieceType::PAWN,PieceType::PAWN,PieceType::PAWN,PieceType::PAWN,PieceType::PAWN,PieceType::PAWN,PieceType::PAWN,PieceType::ROOK,PieceType::KNIGHT,PieceType::BISHOP,PieceType::QUEEN,PieceType::KING,PieceType::BISHOP,PieceType::KNIGHT,PieceType::ROOK}),
        m_WhitePieceTypes({PieceType::PAWN,PieceType::PAWN,PieceType::PAWN,PieceType::PAWN,PieceType::PAWN,PieceType::PAWN,PieceType::PAWN,PieceType::PAWN,PieceType::ROOK,PieceType::KNIGHT,PieceType::BISHOP,PieceType::QUEEN,PieceType::KING,PieceType::BISHOP,PieceType::KNIGHT,PieceType::ROOK})
{
    if (isDefaultBoard)
    {
        m_BlackPieceLocations = std::array<uint8, 16>({A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8});
        m_WhitePieceLocations = std::array<uint8, 16>({A2, B2, C2, D2, E2, F2, G2, H2, A1, B1, C1, D1, E1, F1, G1, H1});
    }
    else
    {
        m_BlackPieceLocations.fill(UNDEFINED_LOCATION);
        m_WhitePieceLocations.fill(UNDEFINED_LOCATION);
    }

    m_Board.fill(EMPTY);
}

Board::Board(const std::string& FEN)
{

}


void Board::MovePiece(const PieceColor color, const PieceID pieceID, const uint8_t targetBoardLocation)
{
    if (targetBoardLocation < 0 || targetBoardLocation > 63)
    {
        throw std::out_of_range("targetBoardLocation is not between 0 and 63");
    }

    if (color == PieceColor::WHITE)
    {
        m_WhitePieceLocations[pieceID] = targetBoardLocation;
    }
    else
    {
        m_BlackPieceLocations[pieceID] = targetBoardLocation;
    }
}


void Board::TakePiece(const PieceColor color, const PieceID pieceID, const uint8_t targetBoardLocation)
{

}


void Board::PromotePawn(const PieceColor color, const PieceID pieceIndex, const PieceType promotionType) const
{

}


uint8 Board::ReadBoardLocation(const PieceColor color, const PieceID pieceID) const
{
    if (color == PieceColor::WHITE)
    {
        return m_WhitePieceLocations[pieceID];
    }
    else
    {
        return m_BlackPieceLocations[pieceID];
    }
}


void Board::WriteBoardLocation(const PieceColor color, const PieceID pieceID, const uint8_t boardLocation)
{
    if (color == PieceColor::WHITE)
    {
        m_WhitePieceLocations[pieceID] = boardLocation;
    }
    else
    {
        m_BlackPieceLocations[pieceID] = boardLocation;
    }
}



