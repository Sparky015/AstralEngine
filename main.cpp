#include "pch.h"
#include "Core/Events/EventListener.h"
#include "Core/Events/EventPublisher.h"

int main()
{
    using namespace Event;

    EventPublisher<int&> pub;
    EventListener<int&> listener = EventListener<int&>([](int& i){LOG(i)});

    EventPublisher<std::string&> pubS;
    EventListener<std::string&> listenerS = EventListener<std::string&>([](std::string& theString){LOG(theString)});
    TRACE("Test");
    EventPublisher<EventListener<int&>&> pubG;
    int f = 5;
    std::string s = "this is a test !!";
    pub.PublishEvent(f);
    pubS.PublishEvent(s);

    return 0;
}
