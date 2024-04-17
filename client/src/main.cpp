//
// Created by sparky on 4/16/2024.
//

#include <Ayla.h>

class Game : public Ayla::Application{
public:
    Game() = default;
    ~Game() = default;
};


Ayla::Application* Ayla::CreateApplication() {
    return new Game();
}