//
// Created by sparky on 4/20/2024.
//

#pragma once
#include <Ayla/EventSystem/Event.h>

namespace Ayla {
    /**
 * @class ApplicationEvent
 * @brief A class that represents an Application Event. It is a subclass of Event.
 */
    class ApplicationEvent : public Event {
    public:
        /**
         * @brief Get the Event Category Flags value
         * @return int The category flags for this event, specified as a bitwise combination of MouseButtonCategory, MouseCategory, and InputCategory.
         */
        int getEventCategoryFlags() override {
            return (MouseButtonCategory | MouseCategory | InputCategory);
        }
    };

/**
 * @class WindowCloseEvent
 * @brief A class that represents a Window Close Event. It is a subclass of ApplicationEvent.
 */
    class WindowCloseEvent : public ApplicationEvent {
    public:
        /**
         * @brief Get the Event Category Flags value
         * @return int The category flags for this event, specified as ApplicationCategory.
         */
        int getEventCategoryFlags() override {
            return ApplicationCategory;
        }

        /**
         * @brief Get the Event Type value
         * @return int The type of this event, specified as WINDOW_CLOSE.
         */
        int getEventType() override {
            return WINDOW_CLOSE;
        }
    };

/**
 * @class WindowResizeEvent
 * @brief A class that represents a Window Resize Event. It is a subclass of ApplicationEvent.
 */
    class WindowResizeEvent : public ApplicationEvent {
    public:
        /**
         * @brief Construct a new Window Resize Event object
         * @param width The new width of the window.
         * @param height The new height of the window.
         */
        WindowResizeEvent(int width, int height) : m_width(width), m_height(height) {}

        /**
         * @brief Get the Event Category Flags value
         * @return int The category flags for this event, specified as ApplicationCategory.
         */
        virtual int getEventCategoryFlags() override {
            return ApplicationCategory;
        }

        /**
         * @brief Get the Event Type value
         * @return int The type of this event, specified as WINDOW_RESIZE.
         */
        virtual int getEventType() override {
            return WINDOW_RESIZE;
        }

        /**
         * @brief Get the width of the resized window
         * @return int Window's new width
         */
        int getWidth() const {
            return m_width;
        }

        /**
         * @brief Get the height of the resized window
         * @return int Window's new height
         */
        int getHeight() const {
            return m_height;
        }

    private:
        int m_width; /**< The new width of the resized window. */
        int m_height; /**< The new height of the resized window. */
    };

    /**
 * @class WindowGainedFocusEvent
 * @brief The WindowGainedFocusEvent class is a type of ApplicationEvent that occurs when the window gains focus.
 */
    class WindowGainedFocusEvent : public ApplicationEvent {
    public:
        /**
         * @brief Get the Event Category Flags value
         * @return int The category flags for this event, specified as ApplicationCategory.
         */
        int getEventCategoryFlags() override {
            return ApplicationCategory;
        }

        /**
         * @brief Get the Event Type value
         * @return int The type of this event, specified as WINDOW_GAINED_FOCUS.
         */
        int getEventType() override {
            return WINDOW_GAINED_FOCUS;
        }
    };

/**
 * @class WindowLostFocusEvent
 * @brief The WindowLostFocusEvent class is a type of ApplicationEvent that occurs when the window loses focus.
 */
    class WindowLostFocusEvent : public ApplicationEvent {
    public:
        /**
         * @brief Get the Event Category Flags value
         * @return int The category flags for this event, specified as ApplicationCategory.
         */
        int getEventCategoryFlags() override {
            return ApplicationCategory;
        }

        /**
         * @brief Get the Event Type value
         * @return int The type of this event, specified as WINDOW_LOST_FOCUS.
         */
        int getEventType() override {
            return WINDOW_LOST_FOCUS;
        }
    };

/**
 * @class WindowMovedEvent
 * @brief WindowMovedEvent class represents an event that is dispatched when a window is moved.
 */
    class WindowMovedEvent : public ApplicationEvent {
    public:
        /**
         * @brief Construct new WindowMovedEvent object
         * @param xpos The new x-position of the window.
         * @param ypos The new y-position of the window.
         */
        WindowMovedEvent(int xpos, int ypos) : m_xpos(xpos), m_ypos(ypos) {}

        /**
         * @brief Get the Event Category Flags value
         * @return int The category flags for this event, specified as ApplicationCategory.
         */
        int getEventCategoryFlags() override {
            return ApplicationCategory;
        }

        /**
         * @brief Get the Event Type value
         * @return int The type of this event, specified as WINDOW_MOVED.
         */
        int getEventType() override {
            return WINDOW_MOVED;
        }

        /**
         * @brief Get the new x-position of the moved window
         * @return int Window's new x-position
         */
        int getXPos() const {
            return m_xpos;
        }

        /**
         * @brief Get the new y-position of the moved window
         * @return int Window's new y-position
         */
        int getYPos() const {
            return m_ypos;
        }

    private:
        int m_xpos; /**< The new x-position of the moved window. */
        int m_ypos; /**< The new y-position of the moved window. */
    };

/**
 * @class AppTickEvent
 * @brief AppTickEvent class represents an application-specific tick event.
 */
    class AppTickEvent : public ApplicationEvent {
    public:
        /**
         * @brief Get the Event Category Flags value
         * @return int The category flags for this event, specified as ApplicationCategory.
         */
        int getEventCategoryFlags() override {
            return ApplicationCategory;
        }

        /**
         * @brief Get the Event Type value
         * @return int The type of this event, specified as APP_TICK.
         */
        int getEventType() override {
            return APP_TICK;
        }
    };

/**
 * @class AppUpdateEvent
 * @brief AppUpdateEvent class represents an event that is dispatched when an application is updated.
 */
    class AppUpdateEvent : public ApplicationEvent {
    public:
        /**
         * @brief Get the Event Category Flags value
         * @return int The category flags for this event, specified as ApplicationCategory.
         */
        int getEventCategoryFlags() override {
            return ApplicationCategory;
        }

        /**
         * @brief Get the Event Type value
         * @return int The type of this event, specified as APP_UPDATE.
         */
        virtual int getEventType() override {
            return APP_UPDATE;
        }
    };

/**
 * @class AppRenderEvent
 * @brief AppRenderEvent class represents an event that is dispatched when an application is rendered.
 */
    class AppRenderEvent : public ApplicationEvent {
    public:
        /**
         * @brief Get the Event Category Flags value
         * @return int The category flags for this event, specified as ApplicationCategory.
         */
        int getEventCategoryFlags() override {
            return ApplicationCategory;
        }

        /**
         * @brief Get the Event Type value
         * @return int The type of this event, specified as APP_RENDER.
         */
        int getEventType() override {
            return APP_RENDER;
        }
    };
}