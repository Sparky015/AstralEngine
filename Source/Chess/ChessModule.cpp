/**
* @file ChessModule.cpp
* @author Andrew Fagan
* @date 12/5/2024
*/

#include "Sozin.h"

#include "Input/InputState.h"
#include "Core/Events/EventListener.h"
#include "ChessBoard/ChessBoardManager.h"
#include "GameFlow/GameManager.h"

class ChessModule : public ApplicationModule
{
public:
    virtual ~ChessModule() = default;

    void Init() override
    {
        TRACE("Initializing Application")
        Game::g_BoardManager.Init();
        Game::g_GameManager.Init();
        keyPressListener.StartListening();
    }

    void Update() override
    {
        if (InputState::IsKeyDown(Keycode::KEY_F))
        {
            LOG("TEST SUCCESSFUL!")
        }
    }

    void Shutdown() override
    {
        TRACE("Shutting down Application")
        Game::g_BoardManager.Shutdown();
        Game::g_GameManager.Shutdown();
    }

    Event::EventListener<KeyPressedEvent> keyPressListener = Event::EventListener<KeyPressedEvent>{
        [](KeyPressedEvent e) { LOG("Key Pressed: " << InputState::GetKeyName(e.keycode)) }
    };
};

IMPLEMENT_APPLICATION_MODULE(ChessModule);