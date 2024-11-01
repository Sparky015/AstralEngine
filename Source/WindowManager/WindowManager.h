//
// Created by Andrew Fagan on 10/31/24.
//

#pragma once
#include "Core/SystemManager.h"

class WindowManager : public SystemManager
{
public:
    WindowManager() = default;
    ~WindowManager() override = default;

    void Update() override;
};