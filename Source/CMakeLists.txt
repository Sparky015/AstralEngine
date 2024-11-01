

set(SOURCE_FILES
        Core/CoreMacroDefinitions.h
        Core/Debug/DebugMacros.h
        Core/Debug/DebugMacros.cpp
        pch.h
        pch.cpp
        Core/Events/EventBus.h
        Core/Events/EventListener.h
        Core/Events/EventPublisher.h
        Game/Board.cpp
        Game/Board.h
        Game/PieceInfo.h
        Game/PieceInfo.cpp
        IOManager/PrintBoardEvent.h
        Core/Events/Event.h
        IOManager/IOManager.cpp
        IOManager/IOManager.h
        Core/Engine.h
        Core/Engine.cpp
        Core/SystemUpdateEvent.h
        Core/SystemManager.h
        Core/SystemManager.cpp
        WindowManager/WindowManager.cpp
        WindowManager/WindowManager.h

)

set(SOURCE_FILES ${SOURCE_FILES} CACHE INTERNAL "Source Files")


add_executable(Chess_Engine
        main.cpp
        ${SOURCE_FILES}
)



target_include_directories(Chess_Engine PRIVATE "${CMAKE_SOURCE_DIR}/Source")
target_precompile_headers(Chess_Engine PRIVATE "${CMAKE_SOURCE_DIR}/Source/pch.h")