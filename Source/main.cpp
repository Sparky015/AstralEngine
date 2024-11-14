#include "Core/Engine.h"
#include "Debug/LogUsedTools.h"

//#include "Debug/AllocationTracker.h"


int main()
{
    {
        Engine engine = Engine();
        Debug::LogEngineDebugInfo();
        engine.Run();
    }

    return 0;
}
