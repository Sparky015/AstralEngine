//
// Created by sparky on 4/16/2024.
//

#include <Ayla.h>

class Game : public Ayla::Core::Application {
public:
    Game() = default;
    ~Game() = default;
    
};


Ayla::Core::Application* Ayla::Core::CreateApplication() {
    return new Game();
}