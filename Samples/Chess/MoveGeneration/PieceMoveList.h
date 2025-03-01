/**
* @file PieceMoveList.h
* @author Andrew Fagan
* @date 12/14/2024
*/

#pragma once

#include "Common/ChessPieceTypes.h"
#include "Movement/ChessMove.h"

namespace Game {

    class PieceMoveList
    {
    public:
        PieceMoveList() = default;
        void AddMove(SquareLocation squareLocation);
        void Clear();
        [[nodiscard]] bool Contains(SquareLocation squareLocation) const;
        [[nodiscard]] bool Contains(ChessMove chessMove) const;
        void ReserveSpace(size_t size);
        [[nodiscard]] inline size_t Size() const {return m_MoveList.size(); }

        inline std::vector<SquareLocation>::iterator begin() { return m_MoveList.begin(); };
        inline std::vector<SquareLocation>::iterator end() { return m_MoveList.end(); }
        SquareLocation& operator[](const size_t index) { return m_MoveList[index]; }
        const SquareLocation& operator[](const size_t index) const { return m_MoveList[index]; }
    private:
        std::vector<SquareLocation> m_MoveList;
    };

}
