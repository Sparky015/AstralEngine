//
// Created by sparky on 4/20/2024.
//

#pragma once
#include <Ayla/EventSystem/Event.h>

namespace Ayla {
    class ApplicationEvent : public Event {
    public:
        virtual int getEventCategoryFlags() override {
            return (MouseButtonCategory | MouseCategory | InputCategory);
        }
    };

    class WindowCloseEvent : public ApplicationEvent {
    public:
        virtual int getEventCategoryFlags() override {
            return ApplicationCategory;
        }
        virtual int getEventType() override{
            return WINDOW_CLOSE;
        }
    };

    class WindowResizeEvent : public ApplicationEvent {
    public:

        WindowResizeEvent(int width, int height) : m_width(width), m_height(height){}

        virtual int getEventCategoryFlags() override {
            return ApplicationCategory;
        }
        virtual int getEventType() override{
            return WINDOW_RESIZE;
        }

        int getWidth() const {
            return m_width;
        }
        int getHeight() const {
            return m_height;
        }

    private:
        int m_width;
        int m_height;
    };

    class WindowGainedFocusEvent : public ApplicationEvent {
    public:
        virtual int getEventCategoryFlags() override {
            return ApplicationCategory;
        }
        virtual int getEventType() override{
            return WINDOW_GAINED_FOCUS;
        }
    };

    class WindowLostFocusEvent : public ApplicationEvent {
    public:
        virtual int getEventCategoryFlags() override {
            return ApplicationCategory;
        }
        virtual int getEventType() override{
            return WINDOW_LOST_FOCUS;
        }
    };

    class WindowMovedEvent : public ApplicationEvent {
    public:
        WindowMovedEvent(int xpos, int ypos) : m_xpos(xpos), m_ypos(ypos) {}

        virtual int getEventCategoryFlags() override {
            return ApplicationCategory;
        }

        virtual int getEventType() override{
            return WINDOW_MOVED;
        }
        int getXPos() const{
            return m_xpos;
        }

        int getYPos() const{
            return m_ypos;
        }

    private:
        int m_xpos;
        int m_ypos;
    };

    class AppTickEvent : public ApplicationEvent {
    public:
        virtual int getEventCategoryFlags() override {
            return ApplicationCategory;
        }
        virtual int getEventType() override{
            return APP_TICK;
        }
    };

    class AppUpdateEvent : public ApplicationEvent {
    public:
        virtual int getEventCategoryFlags() override {
            return ApplicationCategory;
        }
        virtual int getEventType() override{
            return APP_UPDATE;
        }
    };

    class AppRenderEvent : public ApplicationEvent {
    public:
        virtual int getEventCategoryFlags() override {
            return ApplicationCategory;
        }
        virtual int getEventType() override{
            return APP_RENDER;
        }
    };
}