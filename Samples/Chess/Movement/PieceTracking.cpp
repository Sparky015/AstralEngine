/**
* @file PieceTracking.cpp
* @author Andrew Fagan
* @date 12/16/2024
*/

#include "PieceTracking.h"

#include <ChessModule.h>

#include "Chessboard/ChessboardManager.h"
#include "ECS/ECS.h"
#include "ECS/ECSManager.h"
#include "MoveValidation.h"
#include "ChessEntities.h"
#include "Input/InputState.h"
#include "MoveGeneration/KingChecks.h"
#include "Common/ConversionHelpers.h"
#include "Core/Engine.h"


namespace Game {

    void PieceTracking::UpdatePieceTracking()
    {
        if (Astral::InputState::IsKeyDown(Keycode::KEY_LEFT_CLICK))
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
        const Chessboard& chessBoard = ChessModule::Get().GetBoardManager().GetBoard();
        SquareLocation mouseSquareLocation = Game::ConvertCoordinatesToPieceLocation({Astral::InputState::MousePositionX(), 800 - Astral::InputState::MousePositionY()});

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
        SquareLocation mouseSquareLocation = Game::ConvertCoordinatesToPieceLocation({InputState::MousePositionX(), 800 - InputState::MousePositionY()});
        Chessboard& chessBoard = ChessModule::Get().GetBoardManager().GetBoard();

        AttemptMove(chessBoard, mouseSquareLocation);

        // Reset the transform of the tracked piece to the location stored in chessBoard
        ResetTrackedPiecePosition(chessBoard);

        // Reset the mouse and entity
        m_PieceTrackingState = PieceTrackingState::STANDBY;
    }


    void PieceTracking::AlignTrackedPieceWithMouse() const
    {
        if (m_PieceTrackingState != PieceTrackingState::TRACKING) { return; }

        Astral::ECS& ecs = Astral::Engine::Get().GetECSManager().GetECS();

        TransformComponent transformComponent = TransformComponent();
        Astral::ECS_Result result = ecs.GetComponent(m_TrackedPiece.PieceEntity, transformComponent);
        ASSERT(result == Astral::ECS_Result::ECS_SUCCESS, "Failed to get entity component")

        transformComponent.x = Astral::InputState::MousePositionX();
        transformComponent.y = 800 - Astral::InputState::MousePositionY();
        ecs.AddComponent(m_TrackedPiece.PieceEntity, transformComponent);
    }


    void PieceTracking::ResetTrackedPiecePosition(const Chessboard& chessBoard) const
    {
        SquareLocation pieceLocation = chessBoard.GetPieceLocation(m_TrackedPiece.PieceID, m_TrackedPiece.PieceColor);
        Vec2 pieceCoordinates = Game::ConvertPieceLocationToCoordinates(pieceLocation.GetRawValue());

        Astral::ECS& ecs = Astral::Engine::Get().GetECSManager().GetECS();

        // TODO: Fix this temp patch and look up the window dimensions to inverse the transform
        TransformComponent transformComponent;
        Astral::ECS_Result result = ecs.GetComponent(m_TrackedPiece.PieceEntity, transformComponent);
        ASSERT(result == Astral::ECS_Result::ECS_SUCCESS, "Failed to get entity component")

        transformComponent.x = pieceCoordinates.x;
        transformComponent.y = 800 - pieceCoordinates.y;

        // Update the transform component of the piece with the new transform
        ecs.AddComponent(m_TrackedPiece.PieceEntity, transformComponent);
    }


    void PieceTracking::AttemptMove(Chessboard& chessBoard, SquareLocation attemptedMoveLocation) const
    {
        Game::ChessboardManager& boardManager = ChessModule::Get().GetBoardManager();

        if (IsRegularMove(chessBoard, attemptedMoveLocation))
        {
            // Move the piece to the square the mouse was released over
            ChessMove chessMove = ChessMove(m_TrackedPiece.PieceID, m_TrackedPiece.PieceColor, attemptedMoveLocation, MoveType::REGULAR);

            if (IsMoveValid(boardManager.GetBoard(), boardManager.GetMoveList(), chessMove))
            {
                chessBoard.MovePiece(m_TrackedPiece.PieceID, m_TrackedPiece.PieceColor, attemptedMoveLocation);

                // Updating piece move lists after take move
                BoardMoveList& boardMoveList = boardManager.GetMoveList();
                boardMoveList.UpdateMoveList(chessBoard, chessBoard.GetActiveColor().Opposite());
                boardMoveList.UpdateMoveList(chessBoard, chessBoard.GetActiveColor());

                IsKingInCheck(boardManager.GetBoard(), boardMoveList, PieceColor::WHITE);
                IsKingInCheck(boardManager.GetBoard(), boardMoveList, PieceColor::BLACK);
            }
        }
        else if (IsTakeMove(chessBoard, attemptedMoveLocation))
        {
            // Perform take with piece to the square the mouse was released over
            ChessMove chessMove = ChessMove(m_TrackedPiece.PieceID, m_TrackedPiece.PieceColor, attemptedMoveLocation, MoveType::TAKE);

            if (IsMoveValid(boardManager.GetBoard(), boardManager.GetMoveList(), chessMove))
            {
                // Delete entity of taken piece
                Astral::ECS& ecs = Astral::Engine::Get().GetECSManager().GetECS();
                ecs.DeleteEntity(ChessEntities::GetEntity(attemptedMoveLocation));

                chessBoard.CapturePiece(m_TrackedPiece.PieceID, m_TrackedPiece.PieceColor, chessMove.targetLocation);

                // Updating piece move lists after take move
                BoardMoveList& boardMoveList = boardManager.GetMoveList();
                boardMoveList.UpdateMoveList(chessBoard, chessBoard.GetActiveColor().Opposite());
                boardMoveList.UpdateMoveList(chessBoard, chessBoard.GetActiveColor());

                IsKingInCheck(boardManager.GetBoard(), boardMoveList, PieceColor::WHITE);
                IsKingInCheck(boardManager.GetBoard(), boardMoveList, PieceColor::BLACK);
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
