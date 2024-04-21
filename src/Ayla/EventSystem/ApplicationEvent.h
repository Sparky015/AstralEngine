//
// Created by sparky on 4/20/2024.
//

#pragma once
#include <Ayla/EventSystem/Event.h>

namespace Ayla {
    class ApplicationEvent : Event {
    public:
        virtual int getEventCategory() override {
            return (MouseButtonCategory | MouseCategory | InputCategory);
        }
    };

    class WindowCloseEvent : ApplicationEvent {
    public:
        virtual int getEventCategory() override {
            return ApplicationCategory;
        }
        virtual int getEventType() override{
            return WINDOW_CLOSE;
        }
    };

    class WindowResizeEvent : ApplicationEvent {
    public:
        virtual int getEventCategory() override {
            return ApplicationCategory;
        }
        virtual int getEventType() override{
            return WINDOW_RESIZE;
        }
    };

    class WindowFocusEvent : ApplicationEvent {
    public:
        virtual int getEventCategory() override {
            return ApplicationCategory;
        }
        virtual int getEventType() override{
            return WINDOW_FOCUS;
        }
    };

    class WindowLostFocusEvent : ApplicationEvent {
    public:
        virtual int getEventCategory() override {
            return ApplicationCategory;
        }
        virtual int getEventType() override{
            return WINDOW_LOST_FOCUS;
        }
    };

    class WindowMovedEvent : ApplicationEvent {
    public:
        virtual int getEventCategory() override {
            return ApplicationCategory;
        }

        virtual int getEventType() override{
            return WINDOW_MOVED;
        }
    };

    class AppTickEvent : ApplicationEvent {
    public:
        virtual int getEventCategory() override {
            return ApplicationCategory;
        }
        virtual int getEventType() override{
            return APP_TICK;
        }
    };

    class AppUpdateEvent : ApplicationEvent {
    public:
        virtual int getEventCategory() override {
            return ApplicationCategory;
        }
        virtual int getEventType() override{
            return APP_UPDATE;
        }
    };

    class AppRenderEvent : ApplicationEvent {
    public:
        virtual int getEventCategory() override {
            return ApplicationCategory;
        }
        virtual int getEventType() override{
            return APP_RENDER;
        }
    };
}