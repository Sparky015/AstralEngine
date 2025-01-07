#include "Core/Engine.h"
#include "Debug/EngineDebugInfoLogger.h"


int main()
{
    LOG("\n---   Entering main()    ---     (static and global object initialization above)\n");
    {
        std::string test = "1234567890123456789012345678901234567890";
    }
    LOG("Compiled On: " << __DATE__ << " " << __TIME__ << "\n");

    {
        Engine engine = Engine();
        Debug::LogEngineDebugInfo(); // Need to set up window and rendering context before calling this.
        engine.Run();
    }

    LOG("\n---   Exiting main() ---     (static and global object destruction below)\n");
    return 0;
}
