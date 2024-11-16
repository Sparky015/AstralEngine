#include "Core/Engine.h"
#include "Debug/LogUsedTools.h"

//#include "Debug/AllocationTracker.h"


int main()
{
    LOG("Starting engine execution! (entered main)");

    {
        Engine engine = Engine();
        Debug::LogEngineDebugInfo(); // Need to set up window and rendering context before calling this.
        engine.Run();
    }

    return 0;
}
