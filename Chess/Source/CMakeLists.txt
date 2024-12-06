
add_executable(Chess
        Game/Board/Board.cpp
        Game/Board/Board.h
        Game/PieceInfo.h
        Game/PieceInfo.cpp
        Game/Board/BoardManager.cpp
        Game/Board/BoardManager.h
        Game/Board/InternalBoardData.cpp
        Game/Pieces/Bishop.h
        Game/Pieces/Bishop.cpp
        Game/Pieces/King.h
        Game/Pieces/King.cpp
        Game/Pieces/Knight.cpp
        Game/Pieces/Pawn.cpp
        Game/Pieces/Queen.cpp
        Game/Pieces/Rook.cpp
        Game/MoveList.cpp
        Game/Conversions.h
        Game/Conversions.cpp
        Game/MoveGenerationHelpers.cpp
        Game/MoveGenerationHelpers.h
        Game/GameManager.h
        Game/GameManager.cpp
        start.cpp
)

target_link_libraries(Chess PRIVATE SozinEngine)
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Chess)

if (WIN32) #MacOS doesn't like this, so I will only do it for windows
    set_target_properties(Chess PROPERTIES LINK_FLAGS "-static")
endif()
