/**
* @file EntryPoint.cpp
* @author Andrew Fagan
* @date 12/5/2024
*/

#include "Sozin.h"

class Chess : public ApplicationModule
{
public:
    void Init() override
    {
        std::cout << "test";
    }
};

IMPLEMENT_APPLICATION_MODULE(Chess);