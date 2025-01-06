/**
* @file ApplicationModule.h
* @author Andrew Fagan
* @date 12/5/2024
*/

#pragma once


namespace Application {

    class ApplicationModule
    {
    public:
        virtual ~ApplicationModule() = default;

        virtual void Init() {};
        virtual void Update() {};
        virtual void Shutdown() {};
    };

    extern ApplicationModule* CreateApplicationModule();

    #define IMPLEMENT_APPLICATION_MODULE(DerivedApplicationModule) Application::ApplicationModule* Application::CreateApplicationModule() { return new DerivedApplicationModule(); }
}