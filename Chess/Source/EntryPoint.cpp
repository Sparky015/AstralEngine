/**
* @file EntryPoint.cpp
* @author Andrew Fagan
* @date 12/5/2024
*/

#include "Sozin.h"

#include "Input/InputState.h"
#include "Core/Events/EventListener.h"
#include "RenderStuff.h"
#include "Game/Board/BoardManager.h"

class Chess : public ApplicationModule
{
public:

    void Init() override
    {
        TRACE("Initializing Application")
        Game::g_BoardManager.Init();
        keyPressListener.StartListening();
        m_RenderStuff.Init();
    }

    void Update() override
    {
        m_RenderStuff.Update();
        if (InputState::IsKeyDown(Keycode::KEY_F))
        {
            LOG("TEST SUCCESSFUL!")
        }
    }

    void Shutdown() override
    {
        TRACE("Shutting down Application")
        Game::g_BoardManager.Shutdown();
        m_RenderStuff.Shutdown();
    }

    Event::EventListener<KeyPressedEvent> keyPressListener = Event::EventListener<KeyPressedEvent>{[](KeyPressedEvent e){LOG("Key Pressed: " << InputState::GetKeyName(e.keycode))}};
    Renderer::RenderStuff m_RenderStuff;
};

IMPLEMENT_APPLICATION_MODULE(Chess);