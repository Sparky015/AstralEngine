
set(CHESS_SOURCE_FILES_RELATIVE
        Chessboard/Chessboard.cpp
        Chessboard/Chessboard.h
        Common/ChessPieceTypes.h
        Common/ChessPieceTypes.cpp
        Chessboard/ChessboardManager.cpp
        Chessboard/ChessboardManager.h
        Chessboard/InternalChessboardData.cpp
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
        Chessboard/FENParser.cpp
        Chessboard/FENParser.h
        Debug/ImGuiComponents/ChessboardComponents.cpp
        Debug/ImGuiComponents/ChessboardComponents.h
        Chessboard/InternalChessboardData.h
        Common/PieceType.h
        Common/PieceType.cpp
        Debug/DebugManager.cpp
        Debug/DebugManager.h
        Common/SquareLocation.cpp
)

set(CHESS_SOURCE_FILES "")
foreach(file ${CHESS_SOURCE_FILES_RELATIVE})
    list(APPEND CHESS_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${file}")
endforeach()

set(CHESS_SOURCE_FILES ${CHESS_SOURCE_FILES} PARENT_SCOPE)

add_executable(Chess
        ${CHESS_SOURCE_FILES}
        ChessModule.cpp)


target_include_directories(Chess PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Chess PUBLIC AstralEngine)
target_compile_definitions(Chess PUBLIC -DCHESS_ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Assets/")


if (WIN32) #MacOS doesn't like this, so I will only do it for windows
    set_target_properties(Chess PROPERTIES LINK_FLAGS "-static")
endif()

add_dependencies(Chess AstralEngine)
