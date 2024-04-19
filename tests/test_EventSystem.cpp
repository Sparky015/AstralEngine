//
// Created by Andrew Fagan on 4/18/24.
//
#include <gtest/gtest.h>
#include "../src/Ayla/EventSystem/Event.h"


void onRequestPrint(Ayla::Event&){
    std::cout << "Test Event";
}

TEST(EventCreation, simpleEvent){
    Ayla::Event simpleEvent = Ayla::Event();
    simpleEvent += onRequestPrint;
    simpleEvent.raiseEvent(simpleEvent);
}