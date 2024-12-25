/**
* @file MouseInputs.cpp
* @author Andrew Fagan
* @date 12/16/2024
*/

#include "PieceTracking.h"

#include "../ChessBoard/ChessBoardManager.h"
#include "GameState/ChessEntities.h"
#include "GameLogic/MoveValidation.h"
#include "Core/CoreMacroDefinitions.h"
#include "ECS/ECS.h"
#include "ECS/ECSManager.h"
#include "Input/InputState.h"
#include "../Utils/ChessConversions.h"


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
        const ChessBoard& chessBoard = g_BoardManager.GetBoard();
        SquareLocation mouseSquareLocation = Game::ConvertCoordinatesToPieceLocation({InputState::MousePositionX(), InputState::MousePositionY()});

        if (IsSquareEmpty(chessBoard, mouseSquareLocation)) { return; }
        if (!IsPieceTurn(chessBoard, mouseSquareLocation)) { return; }

        // Getting info of piece clicked on
        m_TrackedPiece.PieceEntity = ChessEntities::GetEntity(mouseSquareLocation);
        m_TrackedPiece.PieceID = chessBoard.ReadSquarePieceID(mouseSquareLocation);
        m_TrackedPiece.PieceColor = chessBoard.ReadSquareColor(mouseSquareLocation);

        m_PieceTrackingState = PieceTrackingState::TRACKING;

        // Places the piece being tracked onto the mouse
        AlignTrackedPieceWithMouse();
    }


    void PieceTracking::StopTrackingPiece()
    {
        if (m_PieceTrackingState != PieceTrackingState::TRACKING) { return; }

        // The square that the mouse is over
        SquareLocation mouseSquareLocation = Game::ConvertCoordinatesToPieceLocation({InputState::MousePositionX(), InputState::MousePositionY()});
        ChessBoard& chessBoard = g_BoardManager.GetBoard();

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


    void PieceTracking::ResetTrackedPiecePosition(const ChessBoard& chessBoard) const
    {
        SquareLocation pieceLocation = chessBoard.ReadPieceLocation(m_TrackedPiece.PieceColor, m_TrackedPiece.PieceID);
        Vec2 pieceCoordinates = Game::ConvertPieceLocationToCoordinates(pieceLocation.GetRawValue());
        ECS::ECS& ecs = ECS::g_ECSManager.GetECS();

        TransformComponent& transformComponent = ecs.GetComponent<TransformComponent>(m_TrackedPiece.PieceEntity);
        transformComponent.x = pieceCoordinates.x;
        transformComponent.y = pieceCoordinates.y;

        // Update the transform component of the piece with the new transform
        ecs.AddComponent<TransformComponent>(m_TrackedPiece.PieceEntity, TransformComponent(transformComponent));
    }


    void PieceTracking::AttemptMove(ChessBoard& chessBoard, SquareLocation attemptedMoveLocation) const
    {
        if (IsRegularMove(chessBoard, attemptedMoveLocation))
        {
            // Move the piece to the square the mouse was released over
            ChessMove chessMove = ChessMove(m_TrackedPiece.PieceID, m_TrackedPiece.PieceColor, attemptedMoveLocation, MoveType::REGULAR);

            if (IsMoveValid(g_BoardManager.GetBoard(), g_BoardManager.GetMoveList(), chessMove))
            {
                chessBoard.MovePiece(m_TrackedPiece.PieceColor, m_TrackedPiece.PieceID, attemptedMoveLocation);

                // Updating piece move lists after take move
                BoardMoveList& boardMoveList = g_BoardManager.GetMoveList();
                boardMoveList.UpdateMoveList(chessBoard, chessBoard.GetActiveColor());
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

                chessBoard.TakePiece(m_TrackedPiece.PieceColor, m_TrackedPiece.PieceID, chessMove.targetLocation);

                // Updating piece move lists after take move
                BoardMoveList& boardMoveList = g_BoardManager.GetMoveList();
                boardMoveList.UpdateMoveList(chessBoard, chessBoard.GetActiveColor());
            }
        }
    }


    bool PieceTracking::IsRegularMove(const ChessBoard& chessBoard, SquareLocation targetSquareLocation) const
    {
        return chessBoard.ReadSquareType(targetSquareLocation) == PieceType::NONE;
    }


    bool PieceTracking::IsTakeMove(const ChessBoard& chessBoard, SquareLocation targetSquareLocation) const
    {
        return chessBoard.ReadSquareColor(targetSquareLocation) != m_TrackedPiece.PieceColor;
    }


    bool PieceTracking::IsSquareEmpty(const ChessBoard &chessBoard, SquareLocation squareLocation) const
    {
        return chessBoard.ReadSquareType(squareLocation) == PieceType::NONE;
    }


    bool PieceTracking::IsPieceTurn(const ChessBoard &chessBoard, SquareLocation pieceLocation) const
    {
        return chessBoard.ReadSquareColor(pieceLocation) == chessBoard.GetActiveColor();
    }

}
