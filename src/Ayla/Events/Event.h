//
// Created by Andrew Fagan on 3/27/24.
//

#pragma once
#include "Ayla/aypch.h"

namespace Ayla::Events {


    enum EEventTypes : uint8_t
    {
        NONE,
        WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_GAINED_FOCUS, WINDOW_LOST_FOCUS, WINDOW_MOVED, // Wanted by:  ImGui (Final handle)
        APP_TICK, APP_UPDATE, APP_RENDER,
        KEY_PRESSED, KEY_RELEASED, KEY_PRESSED_REPEATING, KEY_TYPED, // Wanted by:  ImGui, Input State (Final handle)
        MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, MOUSE_SCROLLED, MOUSE_CURSOR_MOVED // Wanted by:  ImGui, Input State (Final handle)
    };


    enum EEventCategory : uint8_t
    { // TODO(sparky): Look into implementing enum classes
        None,
        APPLICATION_CATEGORY = 1 << 0, // Wanted by: ImGui (Final handle)
        INPUT_CATEGORY = 1 << 1,  // Wanted by:  ImGui, Input State (Final handle)
        KEYBOARD_CATEGORY = 1 << 2,  // Wanted by:  ImGui, Input State (Final handle)
        MOUSE_CATEGORY = 1 << 3,     // Wanted by:  ImGui, Input State (Final handle)
        MOUSE_BUTTON_CATEGORY = 1 << 4,   // Wanted by:  ImGui, Input State (Final handle)
    };


    class IEvent {
    public:
        IEvent() = default;
        virtual ~IEvent() = default;

        /** Retrieves the category of the event for event filtering during the dispatch of events. */
        [[nodiscard]] virtual int8_t GetEventCategoryFlags() const = 0;

        /** Retrieves the type of the event for dynamic casting of events and event filtering during the dispatch of events. */
        [[nodiscard]] virtual int GetEventType() const = 0;

        /** Checks to see if ANY of the event's category flags matches a given category flag for filtering out events. */
        [[nodiscard]] bool IsInCategory(const EEventCategory category) const;

        /** Checks to see if an event matches both given categories to filter out events. */
        [[nodiscard]] bool IsInBothCategories(const EEventCategory category1, const EEventCategory category2) const;

        /** Checks to see if ALL of the event's category flags matches a given category flag(s) for filtering out events. */
        [[nodiscard]] bool HasExactCategoryMatch(const EEventCategory category) const;

        /** Sets an event as handled in order to mark that it does not need to be propagated any more in the Layer Stack. */
        void SetIsHandled(const bool isHandled);

        /** Checks to see if an event is handled, so the Layer Stack knows when to stop propagating the event. */
        [[nodiscard]] bool IsHandled() const;

    protected:
        bool m_IsHandled = false;
    };

 } // namespace Ayla::Events
