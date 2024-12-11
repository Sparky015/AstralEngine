
add_executable(Chess
        Source/Board/Board.cpp
        Source/Board/Board.h
        Source/PieceInfo.h
        Source/PieceInfo.cpp
        Source/Board/BoardManager.cpp
        Source/Board/BoardManager.h
        Source/Board/InternalBoardData.cpp
        Source/Pieces/Bishop.h
        Source/Pieces/Bishop.cpp
        Source/Pieces/King.h
        Source/Pieces/King.cpp
        Source/Pieces/Knight.cpp
        Source/Pieces/Pawn.cpp
        Source/Pieces/Queen.cpp
        Source/Pieces/Rook.cpp
        Source/MoveList.cpp
        Source/Conversions.h
        Source/Conversions.cpp
        Source/MoveGenerationHelpers.cpp
        Source/MoveGenerationHelpers.h
        Source/GameManager.h
        Source/GameManager.cpp
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