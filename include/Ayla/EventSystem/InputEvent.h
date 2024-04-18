//
// Created by Andrew Fagan on 4/17/24.
//
#pragma once

#include <Ayla/EventSystem/Event.h>
namespace Ayla{

    class InputEvent : public Event {
        int getEventType() override {
            return INPUT_EVENT;
        }
    };
}