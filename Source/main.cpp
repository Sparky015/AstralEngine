#include "Core/Engine.h"

//#include "Debug/AllocationTracker.h"


int main()
{
    {
        Engine engine = Engine();
        engine.Run();
    }

    return 0;
}
