
add_executable(Chess
        ChessBoard/ChessBoard.cpp
        ChessBoard/ChessBoard.h
        GameState/ChessPieceTypes.h
        GameState/ChessPieceTypes.cpp
        ChessBoard/ChessBoardManager.cpp
        ChessBoard/ChessBoardManager.h
        ChessBoard/InternalChessBoardData.cpp
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
        MoveGeneration/Checks.cpp
        MoveGeneration/Checks.h
        GameState/SquareLocation.h
        GameState/PieceColor.h
        ChessBoard/FENParser.cpp
        ChessBoard/FENParser.h
)

target_link_libraries(Chess PRIVATE SozinEngine)
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Chess)

target_include_directories(Chess PRIVATE ".")

if (WIN32) #MacOS doesn't like this, so I will only do it for windows
    set_target_properties(Chess PROPERTIES LINK_FLAGS "-static")
endif()

