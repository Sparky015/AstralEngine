#include "Core/Engine.h"
#include "Debug/EngineDebugInfoLogger.h"
#include "Debug/Instrumentation/FunctionProfiler.h"

//#include "Debug/AllocationTracker.h"
#include "cpuinfo.h"

int main()
{
    LOG("\n---   Entering main()    ---     (static and global object initialization above)\n");

    LOG("Compiled On: " << __DATE__ << " " << __TIME__ << "\n");
    cpuinfo_initialize();
    LOG(cpuinfo_get_package(0)->name);
    {
        Engine engine = Engine();
        Debug::LogEngineDebugInfo(); // Need to set up window and rendering context before calling this.
        // Debug::FunctionProfiler::ProfileFunction(Debug::LogEngineDebugInfo, "LogEngineDebugInfo" , 1000000);
        engine.Run();
    }

    LOG("\n---   Exiting main() ---     (static and global object destruction below)\n");
    return 0;
}
