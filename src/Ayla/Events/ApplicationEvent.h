//
// Created by sparky on 4/20/2024.
//

#pragma once
#include "Ayla/Events/Event.h"

namespace Ayla::Events {


    class ApplicationEvent : public IEvent
    {
    public:

        [[nodiscard]] int8_t GetEventCategoryFlags() const override
        {
            return (APPLICATION_CATEGORY);
        }

    protected:

        /** ApplicationEvent is not meant to be created as a standalone event. */
        ApplicationEvent() = default;
    };


    class WindowCloseEvent : public ApplicationEvent
    {
    public:

        [[nodiscard]] int8_t GetEventCategoryFlags() const override
        {
            return APPLICATION_CATEGORY;
        }


        [[nodiscard]] int GetEventType() const override
        {
            return WINDOW_CLOSE;
        }

    };

    class WindowResizeEvent : public ApplicationEvent
    {
    public:

        WindowResizeEvent(int width, int height) : m_Width(width), m_Height(height) {}

        [[nodiscard]] virtual int8_t GetEventCategoryFlags() const override
        {
            return APPLICATION_CATEGORY;
        }


        [[nodiscard]] virtual int GetEventType() const override
        {
            return WINDOW_RESIZE;
        }


        [[nodiscard]] int GetWidth() const
        {
            return m_Width;
        }


        [[nodiscard]] int GetHeight() const
        {
            return m_Height;
        }

    private:
        int m_Width;
        int m_Height;
    };


    class WindowGainedFocusEvent final : public ApplicationEvent
    {
    public:

        [[nodiscard]] int8_t GetEventCategoryFlags() const override
        {
            return APPLICATION_CATEGORY;
        }


        [[nodiscard]] int GetEventType() const override
        {
            return WINDOW_GAINED_FOCUS;
        }

    };


    class WindowLostFocusEvent final : public ApplicationEvent
    {
    public:

        [[nodiscard]] int8_t GetEventCategoryFlags() const override
        {
            return APPLICATION_CATEGORY;
        }


        [[nodiscard]] int GetEventType() const override
        {
            return WINDOW_LOST_FOCUS;
        }

    };


    class WindowMovedEvent final : public ApplicationEvent
    {
    public:
        WindowMovedEvent(int xpos, int ypos) : m_Xpos(xpos), m_Ypos(ypos) {}

        [[nodiscard]] int8_t GetEventCategoryFlags() const override
        {
            return APPLICATION_CATEGORY;
        }


        [[nodiscard]] int GetEventType() const override
        {
            return WINDOW_MOVED;
        }


        [[nodiscard]] int GetXPos() const
        {
            return m_Xpos;
        }


        [[nodiscard]] int GetYPos() const
        {
            return m_Ypos;
        }

    private:
        int m_Xpos;
        int m_Ypos;
    };


    class AppTickEvent final: public ApplicationEvent
    {
    public:

        [[nodiscard]] int8_t GetEventCategoryFlags() const override
        {
            return APPLICATION_CATEGORY;
        }


        [[nodiscard]] int GetEventType() const override
        {
            return APP_TICK;
        }

    };


    class AppUpdateEvent final : public ApplicationEvent
    {
    public:

        [[nodiscard]] int8_t GetEventCategoryFlags() const override
        {
            return APPLICATION_CATEGORY;
        }


        [[nodiscard]] int GetEventType() const override
        {
            return APP_UPDATE;
        }

    };


    class AppRenderEvent final : public ApplicationEvent {
    public:

        [[nodiscard]] int8_t GetEventCategoryFlags() const override {
            return APPLICATION_CATEGORY;
        }


        [[nodiscard]] int GetEventType() const override {
            return APP_RENDER;
        }

    };

 }  // namespace Ayla::Events