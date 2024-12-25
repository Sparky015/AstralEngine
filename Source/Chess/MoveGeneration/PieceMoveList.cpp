/**
* @file PieceMoveList.cpp
* @author Andrew Fagan
* @date 12/14/2024
*/

#include "PieceMoveList.h"

namespace Game {

    void PieceMoveList::AddMove(SquareLocation squareLocation)
    {
        m_MoveList.push_back(squareLocation);
    }


    void PieceMoveList::Clear()
    {
        m_MoveList.clear();
    }


    bool PieceMoveList::Contains(SquareLocation squareLocation) const
    {
        return std::find(m_MoveList.begin(), m_MoveList.end(), squareLocation) != m_MoveList.end();
    }


    bool PieceMoveList::Contains(ChessMove chessMove) const
    {
        return std::find(m_MoveList.begin(), m_MoveList.end(), chessMove.targetLocation) != m_MoveList.end();
    }


    void PieceMoveList::ReserveSpace(size_t size)
    {
        m_MoveList.reserve(size);
    }

}
