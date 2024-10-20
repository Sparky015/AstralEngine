#include "pch.h"
#include "Core/Events/EventListener.h"
#include "Core/Events/EventPublisher.h"

int main()
{
    using namespace Event;

    EventPublisher<int&> pub;
    EventListener<int&> listener = EventListener<int&>([](int& i){ERROR(i)});
    int f = 5;
    pub.PublishEvent(f);

    return 0;
}
