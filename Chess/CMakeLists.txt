
add_executable(Chess
        Source/Game/Board/Board.cpp
        Source/Game/Board/Board.h
        Source/Game/PieceInfo.h
        Source/Game/PieceInfo.cpp
        Source/Game/Board/BoardManager.cpp
        Source/Game/Board/BoardManager.h
        Source/Game/Board/InternalBoardData.cpp
        Source/Game/Pieces/Bishop.h
        Source/Game/Pieces/Bishop.cpp
        Source/Game/Pieces/King.h
        Source/Game/Pieces/King.cpp
        Source/Game/Pieces/Knight.cpp
        Source/Game/Pieces/Pawn.cpp
        Source/Game/Pieces/Queen.cpp
        Source/Game/Pieces/Rook.cpp
        Source/Game/MoveList.cpp
        Source/Conversions.h
        Source/Conversions.cpp
        Source/Game/MoveGenerationHelpers.cpp
        Source/Game/MoveGenerationHelpers.h
        Source/Game/GameManager.h
        Source/Game/GameManager.cpp
        Source/EntryPoint.cpp
        Source/RenderStuff.cpp
        Source/RenderStuff.h
)

target_link_libraries(Chess PRIVATE SozinEngine)
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Chess)

if (WIN32) #MacOS doesn't like this, so I will only do it for windows
    set_target_properties(Chess PROPERTIES LINK_FLAGS "-static")
endif()


add_subdirectory(Tests)