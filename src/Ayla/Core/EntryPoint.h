//
// Created by sparky on 4/16/2024.
//

#pragma once
#include "Ayla/Core/Application.h"





int main(int argc, char *argv[]) {
    auto client_app = Ayla::Core::CreateApplication();
    client_app->Run();
    delete client_app;
    return 0;
}