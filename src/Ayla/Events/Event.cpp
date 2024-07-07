//
// Created by Andrew Fagan on 4/18/24.
//


#include "Ayla/Events/Event.h"


namespace Ayla::Events
{

    bool IEvent::isInCategory(const Ayla::Events::EventCategory category) const
    {
        return getEventCategoryFlags() & category;
    }


    bool IEvent::isInBothCategories(const Ayla::Events::EventCategory category1, const Ayla::Events::EventCategory category2) const
    {
        return (getEventCategoryFlags() & category1) && (getEventCategoryFlags() & category2);
    }


    bool IEvent::hasExactCategoryMatch(const Ayla::Events::EventCategory category) const
    {
        return (getEventCategoryFlags() & category) == category;
    }


    bool IEvent::isHandled() const
    {
       return m_isHandled;
    }


    void IEvent::setIsHandled(const bool isHandled)
    {
       m_isHandled = isHandled;
    }

}