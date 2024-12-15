
add_executable(Chess
        Source/ChessBoard/ChessBoard.cpp
        Source/ChessBoard/ChessBoard.h
        Source/ChessPieceTypes.h
        Source/ChessPieceTypes.cpp
        Source/ChessBoard/ChessBoardManager.cpp
        Source/ChessBoard/ChessBoardManager.h
        Source/ChessBoard/InternalChessBoardData.cpp
        Source/MoveGeneration/PieceMoveLists/BishopMoveList.h
        Source/MoveGeneration/PieceMoveLists/BishopMoveList.cpp
        Source/MoveGeneration/PieceMoveLists/KingMoveList.h
        Source/MoveGeneration/PieceMoveLists/KingMoveList.cpp
        Source/MoveGeneration/PieceMoveLists/KnightMoveList.cpp
        Source/MoveGeneration/PieceMoveLists/PawnMoveList.cpp
        Source/MoveGeneration/PieceMoveLists/QueenMoveList.cpp
        Source/MoveGeneration/PieceMoveLists/RookMoveList.cpp
        Source/MoveGeneration/BoardMoveList.cpp
        Source/Utils/ChessConversions.h
        Source/Utils/ChessConversions.cpp
        Source/MoveGeneration/MoveListGenerationUtils.cpp
        Source/ChessGameFlow/GameManager.h
        Source/ChessGameFlow/GameManager.cpp
        Source/ChessModule.cpp
        Source/ChessGameState/ChessEntities.cpp
        Source/ChessGameState/ChessEntities.h
        Source/MoveGeneration/MoveListGenerationUtils.h
        Source/MoveGeneration/BoardMoveList.h
        Source/MoveGeneration/PieceMoveLists/RookMoveList.h
        Source/MoveGeneration/PieceMoveLists/QueenMoveList.h
        Source/MoveGeneration/PieceMoveLists/PawnMoveList.h
        Source/MoveGeneration/PieceMoveLists/KnightMoveList.h
        Source/MoveGeneration/PieceMoveLists/ChessPieceMoveList.h
        Source/ChessLogic/MoveValidation.cpp
        Source/ChessLogic/MoveValidation.h
        Source/MoveGeneration/PieceMoveList.h
        Source/MoveGeneration/PieceMoveList.cpp
        Source/ChessMove.h
)

target_link_libraries(Chess PRIVATE SozinEngine)
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Chess)

target_include_directories(Chess PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source)

if (WIN32) #MacOS doesn't like this, so I will only do it for windows
    set_target_properties(Chess PROPERTIES LINK_FLAGS "-static")
endif()


add_subdirectory(Tests)