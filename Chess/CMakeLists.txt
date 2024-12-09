
add_executable(Chess
        ../SozinEngine/Source/Game/Board/Board.cpp
        ../SozinEngine/Source/Game/Board/Board.h
        ../SozinEngine/Source/Game/PieceInfo.h
        ../SozinEngine/Source/Game/PieceInfo.cpp
        ../SozinEngine/Source/Game/Board/BoardManager.cpp
        ../SozinEngine/Source/Game/Board/BoardManager.h
        ../SozinEngine/Source/Game/Board/InternalBoardData.cpp
        ../SozinEngine/Source/Game/Pieces/Bishop.h
        ../SozinEngine/Source/Game/Pieces/Bishop.cpp
        ../SozinEngine/Source/Game/Pieces/King.h
        ../SozinEngine/Source/Game/Pieces/King.cpp
        ../SozinEngine/Source/Game/Pieces/Knight.cpp
        ../SozinEngine/Source/Game/Pieces/Pawn.cpp
        ../SozinEngine/Source/Game/Pieces/Queen.cpp
        ../SozinEngine/Source/Game/Pieces/Rook.cpp
        ../SozinEngine/Source/Game/MoveList.cpp
        ../SozinEngine/Source/Input/Conversions.h
        ../SozinEngine/Source/Input/Conversions.cpp
        ../SozinEngine/Source/Game/MoveGenerationHelpers.cpp
        ../SozinEngine/Source/Game/MoveGenerationHelpers.h
        ../SozinEngine/Source/Game/GameManager.h
        ../SozinEngine/Source/Game/GameManager.cpp
        Source/EntryPoint.cpp
)

target_link_libraries(Chess PRIVATE SozinEngine)
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Chess)

if (WIN32) #MacOS doesn't like this, so I will only do it for windows
    set_target_properties(Chess PROPERTIES LINK_FLAGS "-static")
endif()


add_subdirectory(Tests)