/**
* @file EventSystemTest.cpp
* @author Andrew Fagan
* @date 10/24/2024
*/

#include <gtest/gtest.h>

#include "Core/Events/EventPublisher.h"
#include "Core/Events/EventListener.h"
#include "Core/Events/EventBus.h"

using namespace Astral;

// example struct
struct Vector
{
    int x;
    int y;
    int z;

    explicit Vector(int x, int y, int z) : x{x}, y{y}, z{z} {}
    bool operator==(const Vector& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
};

class EventSystemTest : public ::testing::Test
{
public:
    void SetUp() override
    {
        Listener.StartListening();
    }
    EventListener<Vector&> Listener = EventListener<Vector&>([this](Vector& v){ this->result = v;});
    EventPublisher<Vector&> Publisher = EventPublisher<Vector&>();
    Vector result = Vector(0, 0, 0);
};

TEST_F(EventSystemTest, SystemTestBasic1)
{
    Vector v = Vector(5,4,1);
    Publisher.PublishEvent(v);
    EXPECT_EQ(v, result);
}

TEST_F(EventSystemTest, SystemTestBasic2)
{
    Vector u = Vector(-1,0,1);
    Publisher.PublishEvent(u);

    Vector v = Vector(1,2,3);
    Publisher.PublishEvent(v);
    EXPECT_EQ(v, result);
}

TEST_F(EventSystemTest, SystemTestWithMoreThanOneListener1)
{
    EventListener<Vector&> eventListener = EventListener<Vector&>([](Vector& v){});
    eventListener.StartListening();
    Vector v = Vector(1,0,-1);
    Publisher.PublishEvent(v);
    EXPECT_EQ(v, result);
}

TEST_F(EventSystemTest, SystemTestWithMoreThanOneListener2)
{
    EventListener<Vector&> eventListener = EventListener<Vector&>([](Vector& v){});
    EventListener<Vector&> eventListener2 = EventListener<Vector&>([](Vector& v){});
    EventListener<Vector&> eventListener3 = EventListener<Vector&>([](Vector& v){});
    EventListener<Vector&> eventListener4 = EventListener<Vector&>([](Vector& v){});
    EventListener<Vector&> eventListener5 = EventListener<Vector&>([](Vector& v){});
    EventListener<Vector&> eventListener6 = EventListener<Vector&>([](Vector& v){});
    EventListener<Vector&> eventListener7 = EventListener<Vector&>([](Vector& v){});
    EventListener<Vector&> eventListener8 = EventListener<Vector&>([](Vector& v){});
    eventListener.StartListening();
    eventListener2.StartListening();
    eventListener3.StartListening();
    eventListener4.StartListening();
    eventListener5.StartListening();
    eventListener6.StartListening();
    eventListener7.StartListening();
    eventListener8.StartListening();

    Vector u = Vector(-1,0,1);
    Publisher.PublishEvent(u);

    Vector v = Vector(-5,-4,1);
    Publisher.PublishEvent(v);
    EXPECT_EQ(v, result);
}

TEST_F(EventSystemTest, RemoveOneListener)
{
    {
        EventListener<Vector&> eventListener = EventListener<Vector&>([](Vector& v){});
        eventListener.StartListening();
    }

    Vector v = Vector(1,0,-1);
    Publisher.PublishEvent(v);
    EXPECT_EQ(v, result);
}

TEST_F(EventSystemTest, RemoveMultipleListeners)
{
    {
        EventListener<Vector&> eventListener = EventListener<Vector&>([](Vector& v){});
        EventListener<Vector&> eventListener2 = EventListener<Vector&>([](Vector& v){});
        EventListener<Vector&> eventListener3 = EventListener<Vector&>([](Vector& v){});
        EventListener<Vector&> eventListener4 = EventListener<Vector&>([](Vector& v){});
        eventListener.StartListening();
        eventListener2.StartListening();
        eventListener3.StartListening();
        eventListener4.StartListening();
    }

    Vector v = Vector(1,0,-1);
    Publisher.PublishEvent(v);
    EXPECT_EQ(v, result);
}

TEST_F(EventSystemTest, RemoveMultiplePublishers)
{
    {
        EventPublisher<Vector&> eventPublisher = EventPublisher<Vector&>();
        EventPublisher<Vector&> eventPublisher2 = EventPublisher<Vector&>();
        EventPublisher<Vector&> eventPublisher3 = EventPublisher<Vector&>();
        EventPublisher<Vector&> eventPublisher4 = EventPublisher<Vector&>();
    }

    Vector v = Vector(1,0,-1);
    Publisher.PublishEvent(v);
    EXPECT_EQ(v, result);
}

TEST_F(EventSystemTest, RemoveMultipleListenersAndPublishers)
{
    {
        EventListener<Vector&> eventListener = EventListener<Vector&>([](Vector& v){});
        EventListener<Vector&> eventListener2 = EventListener<Vector&>([](Vector& v){});
        EventListener<Vector&> eventListener3 = EventListener<Vector&>([](Vector& v){});
        EventListener<Vector&> eventListener4 = EventListener<Vector&>([](Vector& v){});
        eventListener.StartListening();
        eventListener2.StartListening();
        eventListener3.StartListening();
        eventListener4.StartListening();
    }

    {
        EventPublisher<Vector&> eventPublisher = EventPublisher<Vector&>();
        EventPublisher<Vector&> eventPublisher2 = EventPublisher<Vector&>();
        EventPublisher<Vector&> eventPublisher3 = EventPublisher<Vector&>();
        EventPublisher<Vector&> eventPublisher4 = EventPublisher<Vector&>();
    }

    Vector v = Vector(1,0,-1);
    Publisher.PublishEvent(v);
    EXPECT_EQ(v, result);
}

TEST_F(EventSystemTest, SystemTestWithTwoPublisher)
{
    EventPublisher<Vector&> eventPublisher2 = EventPublisher<Vector&>();
    Vector u = Vector(-24,1222,-1);
    eventPublisher2.PublishEvent(u);
    EventListener<Vector&> eventListener = EventListener<Vector&>([](Vector& v){});
    eventListener.StartListening();
    Vector v = Vector(1,0,-1);
    Publisher.PublishEvent(v);
    EXPECT_EQ(v, result);
}

TEST_F(EventSystemTest, SystemTestWithMultiplePublishers)
{
    EventPublisher<Vector&> eventPublisher2 = EventPublisher<Vector&>();
    Vector u = Vector(-284,1211,-14);
    eventPublisher2.PublishEvent(u);

    EventPublisher<Vector&> eventPublisher3 = EventPublisher<Vector&>();
    Vector g = Vector(-274,67,-18);
    eventPublisher3.PublishEvent(g);

    EventPublisher<Vector&> eventPublisher4 = EventPublisher<Vector&>();
    Vector k = Vector(-324,3,-12);
    eventPublisher4.PublishEvent(k);


    Vector v = Vector(-5,-4,1);
    Publisher.PublishEvent(v);
    EXPECT_EQ(v, result);
}


