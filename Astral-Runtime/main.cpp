#include "Core/Engine.h"
#include "Debug/EngineDebugInfoLogger.h"
#include "Debug/Utilities/Loggers.h"
#include "Debug/MemoryTracking/MemoryTracker.h"

int main()
{
    AE_LOG("\n---   Entering main()    ---     (static and global object initialization above)\n");


    {
        Astral::MemoryTracker::Get().Init();
        Astral::Engine engine = Astral::Engine();
        Astral::LogEngineDebugInfo(); // Need to set up window and rendering context before calling this.
        engine.Run();
    }


    AE_LOG("\n---   Exiting main() ---     (static and global object destruction below)\n");
    return 0;
}