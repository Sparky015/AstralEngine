
add_library(Chess
        Chessboard/ChessBoard.cpp
        Chessboard/ChessBoard.h
        Common/ChessPieceTypes.h
        Common/ChessPieceTypes.cpp
        Chessboard/ChessBoardManager.cpp
        Chessboard/ChessBoardManager.h
        Chessboard/InternalChessBoardData.cpp
        MoveGeneration/PieceMoveLists/BishopMoveList.h
        MoveGeneration/PieceMoveLists/BishopMoveList.cpp
        MoveGeneration/PieceMoveLists/KingMoveList.h
        MoveGeneration/PieceMoveLists/KingMoveList.cpp
        MoveGeneration/PieceMoveLists/KnightMoveList.cpp
        MoveGeneration/PieceMoveLists/PawnMoveList.cpp
        MoveGeneration/PieceMoveLists/QueenMoveList.cpp
        MoveGeneration/PieceMoveLists/RookMoveList.cpp
        MoveGeneration/BoardMoveList.cpp
        Common/ConversionHelpers.h
        Common/ConversionHelpers.cpp
        GameManager.h
        GameManager.cpp
        ChessModule.cpp
        ChessEntities.cpp
        ChessEntities.h
        MoveGeneration/MoveListGenerationUtils.h
        MoveGeneration/BoardMoveList.h
        MoveGeneration/PieceMoveLists/RookMoveList.h
        MoveGeneration/PieceMoveLists/QueenMoveList.h
        MoveGeneration/PieceMoveLists/PawnMoveList.h
        MoveGeneration/PieceMoveLists/KnightMoveList.h
        MoveGeneration/PieceMoveLists/ChessPieceMoveList.h
        Movement/MoveValidation.cpp
        Movement/MoveValidation.h
        MoveGeneration/PieceMoveList.h
        MoveGeneration/PieceMoveList.cpp
        Movement/ChessMove.h
        Movement/PieceTracking.cpp
        Movement/PieceTracking.h
        MoveGeneration/KingChecks.cpp
        MoveGeneration/KingChecks.h
        Common/SquareLocation.h
        Common/PieceColor.h
        ChessBoard/FENParser.cpp
        ChessBoard/FENParser.h
        Debug/ImGuiComponents/ChessboardComponents.cpp
        Debug/ImGuiComponents/ChessboardComponents.h
        Chessboard/InternalChessBoardData.h
        Common/PieceType.h
        Common/PieceType.cpp
        Debug/DebugManager.cpp
        Debug/DebugManager.h
        Common/SquareLocation.cpp
)

add_executable(ChessExecutable ChessModule.cpp)
target_link_libraries(ChessExecutable PUBLIC Chess)


target_link_libraries(Chess PUBLIC AstralEngine)

target_include_directories(Chess PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")


if (WIN32) #MacOS doesn't like this, so I will only do it for windows
    set_target_properties(ChessExecutable PROPERTIES LINK_FLAGS "-static")
endif()

add_dependencies(Chess AstralEngine)
add_dependencies(ChessExecutable Chess)
