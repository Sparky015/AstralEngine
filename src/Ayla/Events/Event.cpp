//
// Created by Andrew Fagan on 4/18/24.
//


#include "Ayla/Events/Event.h"


namespace Ayla::Events
{

    bool IEvent::IsInCategory(const EEventCategory category) const
    {
        return (GetEventCategoryFlags() & category) != 0;
    }


    bool IEvent::IsInBothCategories(const EEventCategory category1, const EEventCategory category2) const
    {
        return ((GetEventCategoryFlags() & category1) != 0) && ((GetEventCategoryFlags() & category2) != 0);
    }


    bool IEvent::HasExactCategoryMatch(const EEventCategory category) const
    {
        return (GetEventCategoryFlags() & category) == category;
    }


    bool IEvent::IsHandled() const
    {
       return m_isHandled;
    }


    void IEvent::SetIsHandled(const bool isHandled)
    {
       m_isHandled = isHandled;
    }

} // namespace Ayla::Events