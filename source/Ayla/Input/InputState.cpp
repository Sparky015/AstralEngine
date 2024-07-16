//
// Created by Andrew Fagan on 5/16/24.
//

#include "InputState.h"
#include "Ayla/Events/ApplicationEvent.h"
#include "Ayla/Events/KeyEvent.h"
#include "Ayla/Events/MouseEvent.h"
#include "Ayla/Input/Keycodes.h"


namespace Ayla::Input {

    using namespace Ayla::Events;

    SInputState* SInputState::m_Instance = nullptr;

    SInputState::SInputState()
    {
        AY_TRACE("[Sholas] InputState: Initializing Input State");
        m_InputLayer.SetCallback(std::bind(&SInputState::OnEvent, this, std::placeholders::_1));
    }


    SInputState::~SInputState()
    {
        AY_TRACE("[Sholas] InputState: Destroying Input State");
    }


    void SInputState::Init()
    {
        AY_ASSERT(m_Instance == nullptr, "Input/InputState.cpp: Input State has already been initialized!");
        m_Instance = new SInputState();
    }


    SInputState& SInputState::Get()
    {
        AY_ASSERT(m_Instance != nullptr, "Input/InputState.cpp: Input State has not been initialized yet!");
        return *m_Instance;
    }


    void SInputState::Destroy()
    {
        delete m_Instance;
        m_Instance = nullptr;
    }


    void SInputState::OnEvent(IEvent& event)
    {
        AY_ASSERT(event.IsInCategory(INPUT_CATEGORY), "Input/InputState.cpp: InputState received an event that is not in the Input Category!");

        if (event.IsInCategory(KEYBOARD_CATEGORY))
        {
            if (event.GetEventType() == KEY_PRESSED)
            {
                auto keyPressedEvent = dynamic_cast<KeyPressedEvent&>(event);
                m_KeyState[keyPressedEvent.GetKeycode()].IsDown = true;
            }
            if (event.GetEventType() == KEY_RELEASED)
            {
                auto keyReleasedEvent = dynamic_cast<KeyReleasedEvent&>(event);
                m_KeyState[keyReleasedEvent.GetKeycode()].IsDown = false;
                m_KeyState[keyReleasedEvent.GetKeycode()].IsRepeating = false;
            }
            if (event.GetEventType() == KEY_PRESSED_REPEATING)
            {
                auto keyPressedRepeatingEvent = dynamic_cast<KeyPressedRepeatingEvent&>(event);
                m_KeyState[keyPressedRepeatingEvent.GetKeycode()].IsRepeating = true;
            }
        }

        if (event.IsInCategory(MOUSE_CATEGORY))
        {
            if (event.GetEventType() == MOUSE_BUTTON_PRESSED)
            {
                auto mouseButtonPressedEvent = dynamic_cast<MouseButtonPressEvent&>(event);
                m_KeyState[mouseButtonPressedEvent.GetButton()].IsDown = true;
                //std::cout << mouseButtonPressedEvent.GetButton();
            }
            if (event.GetEventType() == MOUSE_BUTTON_RELEASED)
            {
                auto mouseButtonReleasedEvent = dynamic_cast<MouseButtonReleaseEvent&>(event);
                m_KeyState[mouseButtonReleasedEvent.GetButton()].IsDown = false;
                m_KeyState[mouseButtonReleasedEvent.GetButton()].IsRepeating = false;
                //std::cout << mouseButtonReleasedEvent.GetButton();
            }
            if (event.GetEventType() == MOUSE_CURSOR_MOVED)
            {
                auto mouseMovedEvent = dynamic_cast<MouseMovedEvent&>(event);
                m_MouseCursorState.MouseXPosition = mouseMovedEvent.GetXPos();
                m_MouseCursorState.MouseYPosition = mouseMovedEvent.GetYPos();
                //std::cout << mouseMovedEvent.GetXPos() << ", " << mouseMovedEvent.GetYPos();
            }
            if (event.GetEventType() == MOUSE_SCROLLED)
            {
                auto mouseScrolledEvent = dynamic_cast<MouseScrollEvent&>(event);
                //std::cout << mouseScrolledEvent.getXOffset() << ", " << mouseScrolledEvent.getYOffset();
            }
        }

        event.SetIsHandled(true);
    }


    double SInputState::MousePositionX() const
    {
        return m_MouseCursorState.MouseXPosition;
    }


    double SInputState::MousePositionY() const
    {
        return m_MouseCursorState.MouseYPosition;
    }


    bool SInputState::IsKeyDown(int keycode) const
    {
#ifdef AYLA_DEBUG_BUILD
        if (keycode < 0 || keycode > NUMBER_OF_KEYS - 1)
        {
            AY_WARN("Ayla::Input::SInputState::IsKeyDown: Unrecognized keycode tried to access the input state");
            return false;
        }
#endif
        return m_KeyState[keycode].IsDown;
    }


    bool SInputState::IsKeyRepeating(int keycode) const
    {
#ifdef AYLA_DEBUG_BUILD
        if (keycode < 0 || keycode > NUMBER_OF_KEYS - 1)
        {
            AY_WARN("Ayla::Input::SInputState::IsKeyRepeating: Unrecognized keycode tried to access the input state");
            return false;
        }
#endif
        return m_KeyState[keycode].IsRepeating;
    }


    std::string SInputState::GetKeyName(int keycode) const
    {
#ifdef AYLA_DEBUG_BUILD
        if (keycode < 0 || keycode > NUMBER_OF_KEYS - 1)
        {
            AY_WARN("Ayla::Input::SInputState::GetKeyName: Unrecognized keycode tried to access the input state");
            return m_KeyState[AY_KEY_NONE].Name;
        }
#endif
        return m_KeyState[keycode].Name;
    }

} // namespace Ayla::Input


