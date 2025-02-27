/**
* @file PieceTracking.cpp
* @author Andrew Fagan
* @date 12/16/2024
*/

#include "PieceTracking.h"

#include "Chessboard/ChessboardManager.h"
#include "ECS/ECS.h"
#include "ECS/ECSManager.h"
#include "MoveValidation.h"
#include "ChessEntities.h"
#include "Input/InputState.h"
#include "MoveGeneration/KingChecks.h"
#include "Common/ConversionHelpers.h"


namespace Game {

    void PieceTracking::UpdatePieceTracking()
    {
        if (InputState::IsKeyDown(Keycode::KEY_LEFT_CLICK))
        {
            if (m_PieceTrackingState == PieceTrackingState::STANDBY)
            {
                StartTrackingPiece();
            }

            AlignTrackedPieceWithMouse();
        }
        else
        {
            StopTrackingPiece();
        }
    }


    void PieceTracking::StartTrackingPiece()
    {
        const Chessboard& chessBoard = g_BoardManager.GetBoard();
        SquareLocation mouseSquareLocation = Game::ConvertCoordinatesToPieceLocation({InputState::MousePositionX(), InputState::MousePositionY()});

        if (IsSquareEmpty(chessBoard, mouseSquareLocation)) { return; }
        if (!IsPieceTurn(chessBoard, mouseSquareLocation)) { return; }

        // Getting info of piece clicked on
        m_TrackedPiece.PieceEntity = ChessEntities::GetEntity(mouseSquareLocation);
        m_TrackedPiece.PieceID = chessBoard.GetSquarePieceID(mouseSquareLocation);
        m_TrackedPiece.PieceColor = chessBoard.GetSquareColor(mouseSquareLocation);

        m_PieceTrackingState = PieceTrackingState::TRACKING;

        // Places the piece being tracked onto the mouse
        AlignTrackedPieceWithMouse();
    }


    void PieceTracking::StopTrackingPiece()
    {
        if (m_PieceTrackingState != PieceTrackingState::TRACKING) { return; }

        // The square that the mouse is over
        SquareLocation mouseSquareLocation = Game::ConvertCoordinatesToPieceLocation({InputState::MousePositionX(), InputState::MousePositionY()});
        Chessboard& chessBoard = g_BoardManager.GetBoard();

        AttemptMove(chessBoard, mouseSquareLocation);

        // Reset the transform of the tracked piece to the location stored in chessBoard
        ResetTrackedPiecePosition(chessBoard);

        // Reset the mouse and entity
        m_PieceTrackingState = PieceTrackingState::STANDBY;
    }


    void PieceTracking::AlignTrackedPieceWithMouse() const
    {
        if (m_PieceTrackingState != PieceTrackingState::TRACKING) { return; }
        ECS::ECS& ecs = ECS::g_ECSManager.GetECS();
        TransformComponent& transformComponent = ecs.GetComponent<TransformComponent>(m_TrackedPiece.PieceEntity);

        transformComponent.x = InputState::MousePositionX();
        transformComponent.y = InputState::MousePositionY();
        ecs.AddComponent<TransformComponent>(m_TrackedPiece.PieceEntity, TransformComponent(transformComponent));
    }


    void PieceTracking::ResetTrackedPiecePosition(const Chessboard& chessBoard) const
    {
        SquareLocation pieceLocation = chessBoard.GetPieceLocation(m_TrackedPiece.PieceID, m_TrackedPiece.PieceColor);
        Vec2 pieceCoordinates = Game::ConvertPieceLocationToCoordinates(pieceLocation.GetRawValue());
        ECS::ECS& ecs = ECS::g_ECSManager.GetECS();

        TransformComponent& transformComponent = ecs.GetComponent<TransformComponent>(m_TrackedPiece.PieceEntity);
        transformComponent.x = pieceCoordinates.x;
        transformComponent.y = pieceCoordinates.y;

        // Update the transform component of the piece with the new transform
        ecs.AddComponent<TransformComponent>(m_TrackedPiece.PieceEntity, TransformComponent(transformComponent));
    }


    void PieceTracking::AttemptMove(Chessboard& chessBoard, SquareLocation attemptedMoveLocation) const
    {
        if (IsRegularMove(chessBoard, attemptedMoveLocation))
        {
            // Move the piece to the square the mouse was released over
            ChessMove chessMove = ChessMove(m_TrackedPiece.PieceID, m_TrackedPiece.PieceColor, attemptedMoveLocation, MoveType::REGULAR);

            if (IsMoveValid(g_BoardManager.GetBoard(), g_BoardManager.GetMoveList(), chessMove))
            {
                chessBoard.MovePiece(m_TrackedPiece.PieceID, m_TrackedPiece.PieceColor, attemptedMoveLocation);

                // Updating piece move lists after take move
                BoardMoveList& boardMoveList = g_BoardManager.GetMoveList();
                boardMoveList.UpdateMoveList(chessBoard, chessBoard.GetActiveColor().Opposite());
                boardMoveList.UpdateMoveList(chessBoard, chessBoard.GetActiveColor());

                IsKingInCheck(g_BoardManager.GetBoard(), boardMoveList, PieceColor::WHITE);
                IsKingInCheck(g_BoardManager.GetBoard(), boardMoveList, PieceColor::BLACK);
            }
        }
        else if (IsTakeMove(chessBoard, attemptedMoveLocation))
        {
            // Perform take with piece to the square the mouse was released over
            ChessMove chessMove = ChessMove(m_TrackedPiece.PieceID, m_TrackedPiece.PieceColor, attemptedMoveLocation, MoveType::TAKE);

            if (IsMoveValid(g_BoardManager.GetBoard(), g_BoardManager.GetMoveList(), chessMove))
            {
                // Delete entity of taken piece
                ECS::ECS& ecs = ECS::g_ECSManager.GetECS();
                ecs.RemoveEntity(ChessEntities::GetEntity(attemptedMoveLocation));

                chessBoard.CapturePiece(m_TrackedPiece.PieceID, m_TrackedPiece.PieceColor, chessMove.targetLocation);

                // Updating piece move lists after take move
                BoardMoveList& boardMoveList = g_BoardManager.GetMoveList();
                boardMoveList.UpdateMoveList(chessBoard, chessBoard.GetActiveColor().Opposite());
                boardMoveList.UpdateMoveList(chessBoard, chessBoard.GetActiveColor());

                IsKingInCheck(g_BoardManager.GetBoard(), boardMoveList, PieceColor::WHITE);
                IsKingInCheck(g_BoardManager.GetBoard(), boardMoveList, PieceColor::BLACK);
            }
        }
    }


    bool PieceTracking::IsRegularMove(const Chessboard& chessBoard, SquareLocation targetSquareLocation) const
    {
        return chessBoard.GetSquareType(targetSquareLocation) == PieceType::NONE;
    }


    bool PieceTracking::IsTakeMove(const Chessboard& chessBoard, SquareLocation targetSquareLocation) const
    {
        return chessBoard.GetSquareColor(targetSquareLocation) != m_TrackedPiece.PieceColor;
    }


    bool PieceTracking::IsSquareEmpty(const Chessboard &chessBoard, SquareLocation squareLocation) const
    {
        return chessBoard.GetSquareType(squareLocation) == PieceType::NONE;
    }


    bool PieceTracking::IsPieceTurn(const Chessboard &chessBoard, SquareLocation pieceLocation) const
    {
        return chessBoard.GetSquareColor(pieceLocation) == chessBoard.GetActiveColor();
    }

}
