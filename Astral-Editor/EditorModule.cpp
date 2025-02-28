/**
* @file EditorModule.cpp
* @author Andrew Fagan
* @date 2/28/25
*/

#include "Astral.h"

#include "Input/InputState.h"

class EditorModule : public ApplicationModule
{
public:
    ~EditorModule() override = default;

    void Init() override
    {
        PROFILE_SCOPE("EditorModuleInit")
        TRACE("Initializing Editor")
    }

    void Update(const Core::DeltaTime& deltaTime) override
    {
        if (InputState::IsKeyDown(Keycode::KEY_F))
        {
            LOG("Delta Time: " << deltaTime.GetSeconds());
        }
    }

    void Shutdown() override
    {
        PROFILE_SCOPE("EditorModuleShutdown")
        TRACE("Shutting down Editor")
    }

};

IMPLEMENT_APPLICATION_MODULE(EditorModule);