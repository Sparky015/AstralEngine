//
// Created by Andrew Fagan on 5/16/24.
//
#pragma once

#include "Ayla/Input/InputLayer.h"
#include "Ayla/Events/Event.h"


namespace Ayla::Input {

// TODO: Make some delegates for when keys are pressed. Also make an event delegate class to easily to this.
    class SInputState
    {
    public:

        /**  */
        static void Init();

        /**  */
        static SInputState& Get();

        /**  */
        void OnEvent(Events::IEvent&);

        SInputState(const SInputState&) = delete;
        SInputState& operator=(const SInputState&) = delete;
        SInputState(SInputState&&) = delete;
        SInputState& operator=(SInputState&&) = delete;

        /**  */
        struct KeyState
        {
        public:
            KeyState() : IsDown(false), IsRepeating(false), Name("No Name Given"){}
            explicit KeyState(std::string&& name)  : IsDown(false), IsRepeating(false), Name(std::move(name)){}

            bool IsDown;
            bool IsRepeating;
            std::string Name;
        };

        /**  */
        struct MouseCursorState
        {
            double MouseXPosition;
            double MouseYPosition;
        };

        /**  */
        [[nodiscard]] bool IsKeyDown(int keycode) const;

        /**  */
        [[nodiscard]] bool IsKeyRepeating(int keycode) const;

        /**  */
        [[nodiscard]] std::string GetKeyName(int keycode) const;

        /**  */
        [[nodiscard]] double MousePositionX() const;

        /**  */
        [[nodiscard]] double MousePositionY() const;

    private:
        static SInputState* m_Instance;
        SInputState();
        ~SInputState();

        static const int m_NUMBER_OF_KEYS = 79;

        // The indices all match the key codes (so if you change the key codes, change this)
        std::array<KeyState, m_NUMBER_OF_KEYS> m_KeyState = { // Could potentially make this multiple arrays (one array for each field)
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

                KeyState("LEFT SHIFT"),
                KeyState("RIGHT SHIFT"),
                KeyState("LEFT CONTROL"),
                KeyState("RIGHT CONTROL"),
                KeyState("LEFT ALT"),
                KeyState("RIGHT ALT"),
                KeyState("LEFT SUPER"),
                KeyState("RIGHT SUPER"),
        };

        MouseCursorState m_MouseCursorState = {0.0f, 0.0f};

        InputLayer m_InputLayer;
    };

} // namespace Ayla::Input

