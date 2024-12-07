/**
* @file InputState.h
* @author Andrew Fagan
* @date 12/6/2024
*/

#pragma once

struct KeyState
{
public:
    KeyState() : IsDown(false), IsRepeating(false) {}
    bool IsDown;
    bool IsRepeating;
};


/** Data being tracked for the mouse. */
struct MouseCursorState
{
    double MouseXPosition;
    double MouseYPosition;
};
