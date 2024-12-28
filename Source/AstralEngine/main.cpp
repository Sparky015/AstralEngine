#include "Core/Engine.h"
#include "Debug/Loggers/EngineDebugInfoLogger.h"

//#include "Debug/AllocationTracker.h"

int main()
{
    LOG("\n---   Entering main()    ---     (static and global object initialization above)\n");

    LOG("Compiled On: " << __DATE__ << " " << __TIME__ << "\n");

    {
        PROFILE_SCOPE()
    }
    {
        PROFILE_SCOPE()
    }
    {
        PROFILE_SCOPE()
    }

    {
        Engine engine = Engine();
        Debug::LogEngineDebugInfo(); // Need to set up window and rendering context before calling this.
        engine.Run();
    }

    LOG("\n---   Exiting main() ---     (static and global object destruction below)\n");
    return 0;
}
