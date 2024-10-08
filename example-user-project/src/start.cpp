//
// Created by sparky on 4/16/2024.
//

#include "ExampleLayer.h"
#include "Solas.h"

class ClientApplication : public ClientLoop {
public:

    void Update(float deltaTime) override {
        //AY_LOG("[ClientLoop] Application: Update function called!");
//        if (m_InputState.IsKeyDown(Ayla::Input::AY_KEY_A))
//        {
//            m_EntitySystem.EditEntity(m_EntityHandle).m_Position.x -= .05;
//        }
        //AY_LOG_SS("Delta time is: " << deltaTime);
    };

    void Init() override {
        AY_LOG("[ClientLoop] Application: Init function called!");
        el = ExampleLayer();

        m_EntityHandle = m_EntitySystem.AddEntity(Solas::ECS::Entity());
    }

    ExampleLayer el;



private:
    Solas::ECS::EntitySystem& m_EntitySystem{Solas::ECS::EntitySystem::Get()};
    Solas::Input::SInputState& m_InputState{Solas::Input::SInputState::Get()};
    uint8 m_EntityHandle;
};


ClientLoop* Solas::Client::LinkClientToEngine() {
    return new ClientApplication();
}
