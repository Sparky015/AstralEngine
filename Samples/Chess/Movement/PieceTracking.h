/**
* @file PieceTracking.h
* @author Andrew Fagan
* @date 12/16/2024
*/

#pragma once

#include "ECS/Entity.h"
#include "Chessboard/Chessboard.h"
#include "Chessboard/ChessboardManager.h"
#include "Common/ChessPieceTypes.h"

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
        void ResetTrackedPiecePosition(const Chessboard& chessBoard) const;

        void AttemptMove(Chessboard& chessBoard, SquareLocation attemptedMoveLocation) const;

        [[nodiscard]] bool IsRegularMove(const Chessboard& chessBoard, SquareLocation targetSquareLocation) const;
        [[nodiscard]] bool IsTakeMove(const Chessboard& chessBoard, SquareLocation targetSquareLocation) const;
        [[nodiscard]] bool IsSquareEmpty(const Chessboard& chessBoard, SquareLocation squareLocation) const;
        [[nodiscard]] bool IsPieceTurn(const Chessboard& chessBoard, SquareLocation pieceLocation) const;

        TrackedPiece m_TrackedPiece {TrackedPiece((PieceID)255, PieceColor::BLACK, ECS::Entity())};
        PieceTrackingState m_PieceTrackingState {PieceTrackingState::STANDBY};
    };

}
