//
// Created by Andrew Fagan on 11/1/24.
//

#pragma once

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