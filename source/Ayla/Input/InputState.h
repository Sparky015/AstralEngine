//
// Created by Andrew Fagan on 5/16/24.
//
#pragma once

#include "Ayla/Events/Event.h"
#include "Ayla/Input/InputLayer.h"


namespace Ayla::Input {

    /** Data being tracked for each key. */
    struct KeyState
    {
    public:
        KeyState() : IsDown(false), IsRepeating(false), Name("No Name Given"){}
        explicit KeyState(std::string&& name)  : IsDown(false), IsRepeating(false), Name(std::move(name)){}

        bool IsDown;
        bool IsRepeating;
        std::string Name;
    };


    /** Data being tracked for the mouse. */
    struct MouseCursorState
    {
        double MouseXPosition;
        double MouseYPosition;
    };


    /** Stores the state of keys and mouse in the form of a singleton. */
    class SInputState
    {
    public:

        /** Initializes the Input State singleton. */
        static void Init();

        /** Returns the Input State singleton. */
        static SInputState& Get();

        /** Deletes the singleton instance. */
        static void Destroy();

        /**
         * Receives input events and changes the key's state to reflect the input event data.
         * @note Expects the event to be an event of Input type and to contain keycode values between 0 and 119.
         */
        void OnEvent(Events::IEvent&);

        SInputState(const SInputState&) = delete;
        SInputState& operator=(const SInputState&) = delete;
        SInputState(SInputState&&) = delete;
        SInputState& operator=(SInputState&&) = delete;

        /** Checks if a key is down. */
        [[nodiscard]] bool IsKeyDown(int keycode) const;

        /** Checks if a key is sending a repeat signal (if its being continuously held down). */
        [[nodiscard]] bool IsKeyRepeating(int keycode) const;

        /** Returns the name of a key in the form of a std::string. */
        [[nodiscard]] std::string GetKeyName(int keycode) const;

        /** Returns the X position of the mouse cursor. */
        [[nodiscard]] double MousePositionX() const;

        /** Returns the Y position of the mouse cursor. */
        [[nodiscard]] double MousePositionY() const;

        static const int NUMBER_OF_KEYS = 120;

    private:
        static SInputState* m_Instance;
        SInputState();
        ~SInputState();

        // The naming for each indice all match the definition order of the keycodes (so if you change the order of the key codes, change this)
        std::array<KeyState, NUMBER_OF_KEYS> m_KeyState = { // Could potentially make this multiple arrays (one array for each field)
                KeyState("UNRECOGNIZED KEY"),
                KeyState("LEFT CLICK"),
                KeyState("MIDDLE CLICK"),
                KeyState("RIGHT CLICK"),

                KeyState("A"),
                KeyState("B"),
                KeyState("C"),
                KeyState("D"),
                KeyState("E"),
                KeyState("F"),
                KeyState("G"),
                KeyState("H"),
                KeyState("I"),
                KeyState("J"),
                KeyState("K"),
                KeyState("L"),
                KeyState("M"),
                KeyState("N"),
                KeyState("O"),
                KeyState("P"),
                KeyState("Q"),
                KeyState("R"),
                KeyState("S"),
                KeyState("T"),
                KeyState("U"),
                KeyState("V"),
                KeyState("W"),
                KeyState("X"),
                KeyState("Y"),
                KeyState("Z"),

                KeyState("0"),
                KeyState("1"),
                KeyState("2"),
                KeyState("3"),
                KeyState("4"),
                KeyState("5"),
                KeyState("6"),
                KeyState("7"),
                KeyState("8"),
                KeyState("9"),



                KeyState("SPACE"),
                KeyState("ESCAPE"),
                KeyState("ENTER"),
                KeyState("TAB"),
                KeyState("BACKSPACE"),
                KeyState("INSERT"),
                KeyState("DELETE"),
                KeyState("RIGHT"),
                KeyState("LEFT"),
                KeyState("DOWN"),
                KeyState("UP"),
                KeyState("PAGE UP"),
                KeyState("PAGE DOWN"),
                KeyState("HOME"),
                KeyState("END"),
                KeyState("CAPS LOCK"),
                KeyState("SCROLL LOCK"),
                KeyState("NUM LOCK"),
                KeyState("PRINT SCREEN"),
                KeyState("PAUSE"),
                KeyState("APOSTROPHE"),
                KeyState("COMMA"),
                KeyState("MINUS"),
                KeyState("EQUAL"),
                KeyState("BACKSLASH"),
                KeyState("SEMICOLON"),
                KeyState("SLASH"),
                KeyState("LEFT BRACKET"),
                KeyState("RIGHT BRACKET"),
                KeyState("GRAVE ACCENT"),
                KeyState("PERIOD"),

                KeyState("LEFT SHIFT"),
                KeyState("RIGHT SHIFT"),
                KeyState("LEFT CONTROL"),
                KeyState("RIGHT CONTROL"),
                KeyState("LEFT ALT"),
                KeyState("RIGHT ALT"),
                KeyState("LEFT SUPER"),
                KeyState("RIGHT SUPER"),

                KeyState("KP 0"),
                KeyState("KP 1"),
                KeyState("KP 2"),
                KeyState("KP 3"),
                KeyState("KP 4"),
                KeyState("KP 5"),
                KeyState("KP 6"),
                KeyState("KP 7"),
                KeyState("KP 8"),
                KeyState("KP 9"),
                KeyState("KP DECIMAL"),
                KeyState("KP DIVIDE"),
                KeyState("KP MULTIPLY"),
                KeyState("KP SUBTRACT"),
                KeyState("KP ADD"),
                KeyState("KP ENTER"),
                KeyState("KP EQUAL"),

                KeyState("F1"),
                KeyState("F2"),
                KeyState("F3"),
                KeyState("F4"),
                KeyState("F5"),
                KeyState("F6"),
                KeyState("F7"),
                KeyState("F8"),
                KeyState("F9"),
                KeyState("F10"),
                KeyState("F11"),
                KeyState("F12"),
                KeyState("F13"),
                KeyState("F14"),
                KeyState("F15"),
                KeyState("F16"),
                KeyState("F17"),
                KeyState("F18"),
                KeyState("F19"),
                KeyState("F20"),
                KeyState("F21"),
                KeyState("F22"),
                KeyState("F23"),
                KeyState("F24")
        };

        MouseCursorState m_MouseCursorState = {0.0f, 0.0f};

        InputLayer m_InputLayer;
    };

} // namespace Ayla::Input

