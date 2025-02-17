/**
* @file ChessboardComponents.cpp
* @author Andrew Fagan
* @date 12/28/2024
*/
#include "ChessboardComponents.h"

#include <imgui.h>

#include "Chessboard/ChessboardManager.h"
#include "../../Common/ConversionHelpers.h"

void PieceMoveListTableComponent(const Game::Chessboard& chessboard, const Game::BoardMoveList& boardMoveList)
{
    static constexpr ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;

    if (ImGui::TreeNode("Piece Moves"))
    {

        if (ImGui::TreeNode("White"))
        {

            if (ImGui::BeginTable("WhitePieceMoves", 1, flags, ImVec2(500, 130)))
            {
                for (uint8 pieceID = PIECE_1; pieceID != PIECE_16 + 1; pieceID++)
                {
                    ImGui::TableNextColumn();

                    const Game::PieceMoveList& whiteRegularMoves = boardMoveList.GetRegularMoves(static_cast<PieceID>(pieceID), PieceColor::WHITE);
                    const Game::PieceMoveList& whiteAttackMoves = boardMoveList.GetAttackingMoves(static_cast<PieceID>(pieceID), PieceColor::WHITE);

                    std::string regularMoveString = Game::ConvertPieceMoveListToString(whiteRegularMoves);
                    std::string attackingMoveString = Game::ConvertPieceMoveListToString(whiteAttackMoves);

                    const PieceType pieceType = chessboard.GetPieceType(static_cast<PieceID>(pieceID), PieceColor::WHITE);
                    const SquareLocation pieceLocation = chessboard.GetPieceLocation( static_cast<PieceID>(pieceID), PieceColor::WHITE);

                    ImGui::Text("[%s (%s)] Reg:%s Atk:%s",
                        pieceType.ToString().c_str(), pieceLocation.GetChessNotation().c_str(),
                        regularMoveString.c_str(), attackingMoveString.c_str());
                }
                ImGui::EndTable();
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Black"))
        {

            if (ImGui::BeginTable("BlackPieceMoves", 1, flags, ImVec2(500, 130)))
            {

                for (uint8 pieceID = PIECE_1; pieceID != PIECE_16 + 1; pieceID++)
                {
                    ImGui::TableNextColumn();

                    const Game::PieceMoveList& blackRegularMoves = boardMoveList.GetRegularMoves(static_cast<PieceID>(pieceID), PieceColor::BLACK);
                    const Game::PieceMoveList& blackAttackMoves = boardMoveList.GetAttackingMoves(static_cast<PieceID>(pieceID), PieceColor::BLACK);

                    std::string regularMoveString = Game::ConvertPieceMoveListToString(blackRegularMoves);
                    std::string attackingMoveString = Game::ConvertPieceMoveListToString(blackAttackMoves);

                    const PieceType pieceType = chessboard.GetPieceType(static_cast<PieceID>(pieceID), PieceColor::BLACK);
                    const SquareLocation pieceLocation = chessboard.GetPieceLocation( static_cast<PieceID>(pieceID), PieceColor::BLACK);

                    ImGui::Text("[%s (%s)] Reg:%s Atk:%s",
                        pieceType.ToString().c_str(), pieceLocation.GetChessNotation().c_str(),
                        regularMoveString.c_str(), attackingMoveString.c_str());
                }
                ImGui::EndTable();
            }

            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
}
