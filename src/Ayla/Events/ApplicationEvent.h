//
// Created by sparky on 4/20/2024.
//

#pragma once
#include "Ayla/Events/Event.h"

namespace Ayla::Events {

    class ApplicationEvent : public IEvent
    {
    public:

        [[nodiscard]] int getEventCategoryFlags() const override {
            return (ApplicationCategory);
        }
    };


    class WindowCloseEvent : public ApplicationEvent {
    public:

        [[nodiscard]] int getEventCategoryFlags() const override {
            return ApplicationCategory;
        }


        [[nodiscard]] int getEventType() const override {
            return WINDOW_CLOSE;
        }
    };

    class WindowResizeEvent : public ApplicationEvent {
    public:


        WindowResizeEvent(int width, int height) : m_width(width), m_height(height) {}


        [[nodiscard]] virtual int getEventCategoryFlags() const override {
            return ApplicationCategory;
        }

        [[nodiscard]] virtual int getEventType() const override {
            return WINDOW_RESIZE;
        }


        [[nodiscard]] int getWidth() const {
            return m_width;
        }

        [[nodiscard]] int getHeight() const {
            return m_height;
        }

    private:
        int m_width;
        int m_height;
    };


    class WindowGainedFocusEvent final : public ApplicationEvent {
    public:

        [[nodiscard]] int getEventCategoryFlags() const override {
            return ApplicationCategory;
        }


        [[nodiscard]] int getEventType() const override {
            return WINDOW_GAINED_FOCUS;
        }
    };


    class WindowLostFocusEvent final : public ApplicationEvent {
    public:

        [[nodiscard]] int getEventCategoryFlags() const override {
            return ApplicationCategory;
        }


        [[nodiscard]] int getEventType() const override {
            return WINDOW_LOST_FOCUS;
        }
    };


    class WindowMovedEvent final : public ApplicationEvent {
    public:


        WindowMovedEvent(int xpos, int ypos) : m_xpos(xpos), m_ypos(ypos) {}


        [[nodiscard]] int getEventCategoryFlags() const override {
            return ApplicationCategory;
        }


        [[nodiscard]] int getEventType() const override {
            return WINDOW_MOVED;
        }


        [[nodiscard]] int getXPos() const {
            return m_xpos;
        }


        [[nodiscard]] int getYPos() const {
            return m_ypos;
        }

    private:
        int m_xpos;
        int m_ypos;
    };


    class AppTickEvent final: public ApplicationEvent {
    public:

        [[nodiscard]] int getEventCategoryFlags() const override {
            return ApplicationCategory;
        }


        [[nodiscard]] int getEventType() const override {
            return APP_TICK;
        }
    };


    class AppUpdateEvent final : public ApplicationEvent {
    public:

        [[nodiscard]] int getEventCategoryFlags() const override {
            return ApplicationCategory;
        }


        [[nodiscard]] int getEventType() const override {
            return APP_UPDATE;
        }
    };


    class AppRenderEvent final : public ApplicationEvent {
    public:

        [[nodiscard]] int getEventCategoryFlags() const override {
            return ApplicationCategory;
        }


        [[nodiscard]] int getEventType() const override {
            return APP_RENDER;
        }
    };

 }  // namespace Ayla::Events