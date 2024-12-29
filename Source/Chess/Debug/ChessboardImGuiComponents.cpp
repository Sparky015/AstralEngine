/**
* @file ChessboardImGuiComponents.cpp
* @author Andrew Fagan
* @date 12/28/2024
*/
#include "ChessboardImGuiComponents.h"

#include <imgui.h>

#include "Chessboard/ChessboardManager.h"
#include "Utils/ChessConversions.h"

void PieceMoveListTableComponent(const char* tableName, const Game::Chessboard& chessboard, const Game::PieceMoveList& pieceMoveList)
{
    static constexpr ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;

    if (ImGui::BeginTable("WhitePieceMoves", 1, flags, ImVec2(500, 130)))
    {
        const Game::Chessboard& board = Game::g_BoardManager.GetBoard();
        const Game::BoardMoveList& whiteMoveList = Game::g_BoardManager.GetMoveList();
        for (uint8 pieceID = PIECE_1; pieceID != PIECE_16 + 1; pieceID++)
        {
            ImGui::TableNextColumn();

            const Game::PieceMoveList& whiteRegularMoves = whiteMoveList.GetRegularMoves(static_cast<PieceID>(pieceID), PieceColor::WHITE);
            const Game::PieceMoveList& whiteAttackMoves = whiteMoveList.GetAttackingMoves(static_cast<PieceID>(pieceID), PieceColor::WHITE);

            const char* regularMoveStr = Game::ConvertPieceMoveListToString(whiteRegularMoves).c_str();
            const char* attackingMoveStr = Game::ConvertPieceMoveListToString(whiteAttackMoves).c_str();

            const PieceType pieceType = board.GetPieceType(static_cast<PieceID>(pieceID), PieceColor::WHITE);
            const SquareLocation pieceLocation = board.GetPieceLocation( static_cast<PieceID>(pieceID), PieceColor::WHITE);
            ImGui::Text("[%s (%s)] Reg:%s Atk:%s", pieceType.ToString().c_str(), Game::ConvertIntToChessNotation(pieceLocation.GetRawValue()).c_str(), regularMoveStr, attackingMoveStr);
        }
        ImGui::EndTable();
    }
    if (ImGui::BeginTable("BlackPieceMoves", 1, flags, ImVec2(500, 130)))
    {
        const Game::Chessboard& board = Game::g_BoardManager.GetBoard();
        const Game::BoardMoveList& whiteMoveList = Game::g_BoardManager.GetMoveList();
        for (uint8 pieceID = PIECE_1; pieceID != PIECE_16 + 1; pieceID++)
        {
            ImGui::TableNextColumn();

            const Game::PieceMoveList& whiteRegularMoves = whiteMoveList.GetRegularMoves(static_cast<PieceID>(pieceID), PieceColor::WHITE);
            const Game::PieceMoveList& whiteAttackMoves = whiteMoveList.GetAttackingMoves(static_cast<PieceID>(pieceID), PieceColor::WHITE);

            const char* regularMoveStr = Game::ConvertPieceMoveListToString(whiteRegularMoves).c_str();
            const char* attackingMoveStr = Game::ConvertPieceMoveListToString(whiteAttackMoves).c_str();

            const PieceType pieceType = board.GetPieceType(static_cast<PieceID>(pieceID), PieceColor::WHITE);
            const SquareLocation pieceLocation = board.GetPieceLocation( static_cast<PieceID>(pieceID), PieceColor::WHITE);
            ImGui::Text("[%s (%s)] Reg:%s Atk:%s", pieceType.ToString().c_str(), Game::ConvertIntToChessNotation(pieceLocation.GetRawValue()).c_str(), regularMoveStr, attackingMoveStr);
        }
        ImGui::EndTable();
    }
}
