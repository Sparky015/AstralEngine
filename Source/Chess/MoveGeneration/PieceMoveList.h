/**
* @file PieceMoveList.h
* @author Andrew Fagan
* @date 12/14/2024
*/

#pragma once

#include "../GameState/ChessPieceTypes.h"
#include "../ChessMove.h"

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

        inline std::vector<SquareLocation>::iterator begin() { return m_MoveList.begin(); };
        inline std::vector<SquareLocation>::iterator end() { return m_MoveList.end(); }
    private:
        std::vector<SquareLocation> m_MoveList;
    };

}
