/**
* @file MenuComponent.h
* @author Andrew Fagan
* @date 3/29/26
*/

#pragma once

#include <string>

namespace Astral {

    class MenuComponent
    {
    public:
        static void Show();

    private:

        static void SelectFile(std::string& outFilePath);

    };

}