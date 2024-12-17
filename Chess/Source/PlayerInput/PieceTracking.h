/**
* @file MouseInputs.h
* @author Andrew Fagan
* @date 12/16/2024
*/

#pragma once

#include "ChessPieceTypes.h"
#include "ChessBoard/ChessBoard.h"
#include "ChessBoard/ChessBoardManager.h"
#include "ECS/Entity.h"

namespace Game {

    enum class PieceTrackingState : uint8
    {
        STANDBY,
        TRACKING,
    };


    struct TrackedPiece
    {
        TrackedPiece(PieceID pieceID, PieceColor pieceColor, ECS::Entity pieceEntity)
            : PieceID(pieceID), PieceColor(pieceColor), PieceEntity(pieceEntity) {}

        PieceID PieceID;
        PieceColor PieceColor;
        ECS::Entity PieceEntity;
    };


    class PieceTracking
    {
    public:
        void UpdatePieceTracking();

    private:
        void StartTrackingPiece();
        void StopTrackingPiece();

        void AlignTrackedPieceWithMouse() const;
        void ResetTrackedPiecePosition(const ChessBoard& chessBoard) const;

        void AttemptMove(ChessBoard& chessBoard, SquareLocation attemptedMoveLocation) const;
        [[nodiscard]] bool IsRegularMove(const ChessBoard& chessBoard, SquareLocation targetSquareLocation) const;
        [[nodiscard]] bool IsTakeMove(const ChessBoard& chessBoard, SquareLocation targetSquareLocation) const;
        [[nodiscard]] bool IsSquareEmpty(const ChessBoard& chessBoard, SquareLocation squareLocation) const;
        [[nodiscard]] bool IsPieceTurn(const ChessBoard& chessBoard, SquareLocation pieceLocation) const;

        TrackedPiece m_TrackedPiece {TrackedPiece((PieceID)255, PieceColor::BLACK, ECS::Entity())};
        PieceTrackingState m_PieceTrackingState {PieceTrackingState::STANDBY};
    };

}
