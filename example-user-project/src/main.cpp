//
// Created by sparky on 4/16/2024.
//

#include "Ayla.h"
#include "ExampleLayer.h"

class ClientApplication : public ClientLoop {
public:

    void Update() override {
        //AY_LOG("[ClientLoop] Application: Update function called!");
    };

    void Init() override {
        AY_LOG("[ClientLoop] Application: Init function called!");
        el = ExampleLayer();
    }

    ExampleLayer el;
};


ClientLoop* Ayla::Client::LinkClientToEngine() {
    return new ClientApplication();
}
