/**
* @file ProfilerAppModule.cpp
* @author Andrew Fagan
* @date 3/29/26
*/

#include "ProfilerApp.h"

#include "Astral.h"

namespace Astral {

    class ProfilerAppModule : public ApplicationModule
    {
    public:
        ~ProfilerAppModule() override = default;

        void Init() override
        {
            PROFILE_SCOPE("ProfilerAppModule::Init")
            AE_TRACE("Initializing ProfilerApp")

            m_ProfilerApp.Init();
        }

        void Update(const Astral::DeltaTime& deltaTime) override
        {

        }

        void Shutdown() override
        {
            PROFILE_SCOPE("EditorModuleShutdown")
            AE_TRACE("Shutting down Editor")

            m_ProfilerApp.Shutdown();
        }

    private:

        ProfilerApp m_ProfilerApp;
    };

}

IMPLEMENT_APPLICATION_MODULE(Astral::ProfilerAppModule);