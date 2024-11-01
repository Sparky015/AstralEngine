//
// Created by Andrew Fagan on 10/30/24.
//

#include "Core/SystemManager.h"

#pragma once

class IOManager : public SystemManager
{
public:
    IOManager() = default;
    ~IOManager() override = default;

    void Setup();
    void Shutdown();

    void Update() override;
};