/**
* @file EditorModule.cpp
* @author Andrew Fagan
* @date 2/28/25
*/

#include "Astral.h"

#include "Input/InputState.h"

namespace Astral {

    class VulkanPlaygroundModule : public ApplicationModule
    {
    public:
        ~VulkanPlaygroundModule() override = default;

        void Init() override
        {
            PROFILE_SCOPE("EditorModuleInit")
            TRACE("Initializing Editor")
        }

        void Update(const Astral::DeltaTime& deltaTime) override
        {

        }

        void Shutdown() override
        {
            PROFILE_SCOPE("EditorModuleShutdown")
            TRACE("Shutting down Editor")
        }

    };

}

IMPLEMENT_APPLICATION_MODULE(Astral::VulkanPlaygroundModule);