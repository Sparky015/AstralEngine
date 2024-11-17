#include "Core/Engine.h"
#include "Debug/LogUsedTools.h"

//#include "Debug/AllocationTracker.h"


int main()
{
    Debug::Macros::InitLogFileForMacros();
    LOG("\n---   Entering main()    ---     (static and global object initialization above)\n");

    {
        Engine engine = Engine();
        Debug::LogEngineDebugInfo(); // Need to set up window and rendering context before calling this.
        engine.Run();
    }

    LOG("\n---   Exiting main() ---     (static and global object destruction below)\n");
    Debug::Macros::CloseLogFileForMacros();
    return 0;
}
