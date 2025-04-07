#include "Core/Engine.h"
#include "Debug/EngineDebugInfoLogger.h"
#include "Debug/Utilities/Loggers.h"
#include "Core/Memory/Allocators/STLAllocators/STLLinearAllocator.h"

int main()
{
    LOG("\n---   Entering main()    ---     (static and global object initialization above)\n");


    using CustomVector = std::vector<int, Core::STLLinearAllocator<int>>;
    Core::STLLinearAllocator<int> allocator = Core::STLLinearAllocator<int>(200);
    CustomVector intVector{allocator};

    intVector.reserve(3); // Use as normal

    {
        Engine engine = Engine();
        Debug::LogEngineDebugInfo(); // Need to set up window and rendering context before calling this.
        engine.Run();
    }

    LOG("\n---   Exiting main() ---     (static and global object destruction below)\n");
    return 0;
}
