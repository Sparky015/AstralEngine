

set(SOURCE_FILES
        Core/CoreMacroDefinitions.h
        Core/Debug/DebugMacros.h
        Core/Debug/DebugMacros.cpp
        pch.h
        pch.cpp
        Core/Events/EventBus.h
        Core/Events/EventListener.h
        Core/Events/EventPublisher.h
        InputManager/InputManager.cpp
        InputManager/InputManager.h
        Game/Board.cpp
        Game/Board.h
        Game/PieceInfo.h
        CACHE INTERNAL "Source Files"
)

add_executable(Chess_Engine
        main.cpp
        ${SOURCE_FILES}
)



target_include_directories(Chess_Engine PRIVATE "${CMAKE_SOURCE_DIR}/Source")
target_precompile_headers(Chess_Engine PRIVATE "${CMAKE_SOURCE_DIR}/Source/pch.h")