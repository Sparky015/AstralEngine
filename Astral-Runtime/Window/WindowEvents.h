//
// Created by Andrew Fagan on 11/1/24.
//

#pragma once

#include "Core/CoreMacroDefinitions.h"

struct WindowClosedEvent {};

struct KeyPressedEvent
{
    explicit KeyPressedEvent(int keycode) : keycode(keycode) {}
    int keycode;
};


struct KeyReleasedEvent
{
    explicit KeyReleasedEvent(int keycode) : keycode(keycode) {}
    int keycode;
};


struct KeyRepeatingEvent
{
    explicit KeyRepeatingEvent(int keycode) : keycode(keycode) {}
    int keycode;
};


struct MouseMovedEvent
{
    explicit MouseMovedEvent(double xPos, double yPos) : xPosition(xPos), yPosition(yPos) {}
    double xPosition;
    double yPosition;
};


struct WindowResizedEvent
{
    explicit WindowResizedEvent(uint32 newWidth, uint32 newHeight) : Width(newWidth), Height(newHeight) {}
    uint32 Width;
    uint32 Height;
};


struct FramebufferResizedEvent
{
    explicit FramebufferResizedEvent(uint32 newWidth, uint32 newHeight) : Width(newWidth), Height(newHeight) {}
    uint32 Width;
    uint32 Height;
};