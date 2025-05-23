/**
* @file InputState.h
* @author Andrew Fagan
* @date 12/6/2024
*/

#pragma once

#include "Keycodes.h"

#include "Window/WindowEvents.h"
#include "Core/Events/EventListener.h"

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
    MouseCursorState() : MouseXPosition(0.0), MouseYPosition(0.0) {}
    double MouseXPosition;
    double MouseYPosition;
};


class InputState
{
public:
    InputState();

    static void Init();
    static void Shutdown();

    /** Checks if a key is down. */
    [[nodiscard]] static bool IsKeyDown(int keycode);

    /** Checks if a key is sending a repeat signal (if its being continuously held down). */
    [[nodiscard]] static bool IsKeyRepeating(int keycode);

    /** Returns the name of a key in the form of a std::string. */
    [[nodiscard]] static std::string GetKeyName(int keycode);

    /** Returns the X position of the mouse cursor. */
    [[nodiscard]] static double MousePositionX();

    /** Returns the Y position of the mouse cursor. */
    [[nodiscard]] static double MousePositionY();

    static const int NUMBER_OF_KEYS = 120;

private:

    static void OnKeyPress(KeyPressedEvent keyPressedEvent);
    static void OnKeyRelease(KeyReleasedEvent keyReleasedEvent);
    static void OnKeyRepeating(KeyRepeatingEvent keyRepeatingEvent);
    static void OnMouseMoved(MouseMovedEvent mouseMovedEvent);

    static std::array<KeyState, NUMBER_OF_KEYS> m_KeyState;
    static MouseCursorState m_MouseCursorState;
    static Core::EventListener<KeyPressedEvent> m_KeyPressListener;
    static Core::EventListener<KeyReleasedEvent> m_KeyReleaseListener;
    static Core::EventListener<KeyRepeatingEvent> m_KeyRepeatingListener;
    static Core::EventListener<MouseMovedEvent> m_MouseMovedListener;

};
