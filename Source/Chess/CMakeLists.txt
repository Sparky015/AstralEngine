
add_library(Chess
        Chessboard/ChessBoard.cpp
        Chessboard/ChessBoard.h
        GameState/ChessPieceTypes.h
        GameState/ChessPieceTypes.cpp
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
        Utils/ChessConversions.h
        Utils/ChessConversions.cpp
        GameFlow/GameManager.h
        GameFlow/GameManager.cpp
        ChessModule.cpp
        GameState/ChessEntities.cpp
        GameState/ChessEntities.h
        MoveGeneration/MoveListGenerationUtils.h
        MoveGeneration/BoardMoveList.h
        MoveGeneration/PieceMoveLists/RookMoveList.h
        MoveGeneration/PieceMoveLists/QueenMoveList.h
        MoveGeneration/PieceMoveLists/PawnMoveList.h
        MoveGeneration/PieceMoveLists/KnightMoveList.h
        MoveGeneration/PieceMoveLists/ChessPieceMoveList.h
        GameLogic/MoveValidation.cpp
        GameLogic/MoveValidation.h
        MoveGeneration/PieceMoveList.h
        MoveGeneration/PieceMoveList.cpp
        ChessMove.h
        PlayerInput/PieceTracking.cpp
        PlayerInput/PieceTracking.h
        MoveGeneration/KingChecks.cpp
        MoveGeneration/KingChecks.h
        GameState/SquareLocation.h
        GameState/PieceColor.h
        ChessBoard/FENParser.cpp
        ChessBoard/FENParser.h
        Debug/ImGuiComponents/ChessboardComponents.cpp
        Debug/ImGuiComponents/ChessboardComponents.h
        Chessboard/InternalChessBoardData.h
        GameState/PieceType.h
        GameState/PieceType.cpp
        Debug/DebugManager.cpp
        Debug/DebugManager.h
        GameState/SquareLocation.cpp
)

add_executable(ChessExecutable ChessModule.cpp)
target_link_libraries(ChessExecutable PUBLIC Chess)


target_link_libraries(Chess PUBLIC AstralEngine)
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Chess)

target_include_directories(Chess PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")


if (WIN32) #MacOS doesn't like this, so I will only do it for windows
    set_target_properties(ChessExecutable PROPERTIES LINK_FLAGS "-static")
endif()

add_dependencies(Chess AstralEngine)
add_dependencies(ChessExecutable Chess)
