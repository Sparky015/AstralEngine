#include "Core/Engine.h"
#include "Debug/EngineDebugInfoLogger.h"
#include "Core/Utilities/Loggers.h"
#include "Profiler/MemoryTracking/MemoryTracker.h"

int main()
{
    AE_LOG("\n---   Entering main()    ---     (static and global object initialization above)\n");


    {
        Astral::MemoryTracker::Get().Init();
        Astral::Engine engine = Astral::Engine();
        engine.Init();
        Astral::LogEngineDebugInfo(); // Need to set up window and rendering context before calling this.
        engine.Run();
        engine.Shutdown();
        Astral::MemoryTracker::Get().Shutdown();
    }


    AE_LOG("\n---   Exiting main() ---     (static and global object destruction below)\n");
    return 0;
}