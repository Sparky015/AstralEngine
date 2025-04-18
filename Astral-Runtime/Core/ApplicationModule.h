/**
* @file ApplicationModule.h
* @author Andrew Fagan
* @date 12/5/2024
*/

#pragma once

#include "Core/Time/DeltaTime.h"
#include "Core/SmartPointers.h"

namespace Application {

    class ApplicationModule
    {
    public:
        virtual ~ApplicationModule() = default;

        virtual void Init() {};
        virtual void Update(const Core::DeltaTime& deltaTime) {};
        virtual void Shutdown() {};
    };

    extern Astral::ScopedPtr<Application::ApplicationModule> CreateApplicationModule();

    #define IMPLEMENT_APPLICATION_MODULE(DerivedApplicationModule) Astral::ScopedPtr<Application::ApplicationModule> Application::CreateApplicationModule() { return Astral::CreateScopedPtr<DerivedApplicationModule>(); }
}