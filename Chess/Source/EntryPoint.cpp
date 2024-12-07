/**
* @file EntryPoint.cpp
* @author Andrew Fagan
* @date 12/5/2024
*/

#include "Sozin.h"

#include "Input/InputState.h"
#include "Core/Events/EventListener.h"

class Chess : public ApplicationModule
{
public:

    void Init() override
    {
        TRACE("Initializing Application")
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
    }

    Event::EventListener<KeyPressedEvent> keyPressListener = Event::EventListener<KeyPressedEvent>{[](KeyPressedEvent e){LOG("Key Pressed: " << InputState::GetKeyName(e.keycode))}};
};

IMPLEMENT_APPLICATION_MODULE(Chess);