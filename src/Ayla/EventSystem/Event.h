//
// Created by Andrew Fagan on 3/27/24.
//

#pragma once
#include "Ayla/aypch.h"

namespace Ayla {


    enum EventTypes {
        NONE,
        WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_GAINED_FOCUS, WINDOW_LOST_FOCUS, WINDOW_MOVED,
        APP_TICK, APP_UPDATE, APP_RENDER,
        KEY_PRESSED, KEY_RELEASED, KEY_PRESSED_REPEATING,
        MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, MOUSE_SCROLLED, MOUSE_CURSOR_MOVED
    };


    enum EventCategory {
        None,
        ApplicationCategory = 1 < 0,
        InputCategory = 1 < 1,
        KeyboardCategory = 1 < 2,
        MouseCategory = 1 < 3, // <3
        MouseButtonCategory = 1 < 4,
    };


/**
 * @class Event
 *
 * @brief Base class for different types of events.
 *
 * This class is used to handle the different types of events such as window, application, keyboard,
 * mouse events, etc. in Ayla project. The class contains various methods to handle and manage events.
 *
 * @details The Event class contains an enum for EventTypes, and EventCategory,
 *          Also has methods for categorizing and handling events.
 */
    class Event {
    public:

        /**
         * @brief Virtual destructor for the event.
         */
        virtual ~Event() = default;

        /**
         * @brief Gets the flags associated to the event category.
         * @return Returns the flags of the event category as an int.
         */
        virtual int getEventCategoryFlags();

        /**
         * @brief Gets the type of the event.
         * @return Returns the type of the event as an int.
         */
        virtual int getEventType();

        /**
         * @brief Checks if the event is in the given category.
         *
         * @param category The event category to be checked.
         * @return Returns true if the event is in the given category, false otherwise.
         */
        bool isInCategory(EventCategory category) {
            return getEventCategoryFlags() & category;
        }

        /**
         * @brief Sets the handled status of the event.
         * @param isHandled The boolean value to set the handled status of the event.
         */
        void setIsHandled(bool isHandled);

        /**
         * @brief Checks if the event is handled
         * @return Returns true if the event has been handled, false otherwise.
         */
        bool isHandled();

    protected:
        bool m_isHandled = false; /**< A flag to indicate if the event is handled. */
    };

/**
 * @brief A function to process events, based on input taken from the event queue.
 * @param event The event to be processed.
 * @return returns an integer after processing the event.
 */
    int PollEvent(Event &event);

}



