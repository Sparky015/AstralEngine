//
// Created by Andrew Fagan on 4/18/24.
//


#include "Ayla/EventSystem/Event.h"


namespace Ayla{



   int Event::getEventType() { return NONE;}
    int Event::getEventCategoryFlags() {return None;}

    bool Event::isHandled() {
       return m_isHandled;
   }



}