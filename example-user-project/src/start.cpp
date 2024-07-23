//
// Created by sparky on 4/16/2024.
//

#include "Ayla.h"
#include "ExampleLayer.h"

class ClientApplication : public ClientLoop {
public:

    void Update() override {
        //AY_LOG("[ClientLoop] Application: Update function called!");
        if (m_InputState.IsKeyDown(Ayla::Input::AY_KEY_A))
        {
            AY_LOG("TEST");
            m_EntitySystem.EditEntity(m_EntityHandle).m_Position.x -= .05;
        }

    };

    void Init() override {
        AY_LOG("[ClientLoop] Application: Init function called!");
        el = ExampleLayer();

        m_EntityHandle = m_EntitySystem.AddEntity(Ayla::ECS::Entity());
    }

    ExampleLayer el;



private:
    Ayla::ECS::EntitySystem& m_EntitySystem{Ayla::ECS::EntitySystem::Get()};
    Ayla::Input::SInputState& m_InputState{Ayla::Input::SInputState::Get()};
    uint8 m_EntityHandle;
};


ClientLoop* Ayla::Client::LinkClientToEngine() {
    return new ClientApplication();
}
