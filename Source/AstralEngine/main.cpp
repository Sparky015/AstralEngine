#include "Core/Engine.h"
#include "Debug/EngineDebugInfoLogger.h"
#include "Debug/Instrumentation/FunctionProfiler.h"

#include "Core/Memory/AllocationTracker.h"
#include "Core/Memory/MemoryMetricsManager.h"
//#include "Debug/AllocationTracker.h"

// void* operator new(std::size_t size)
// {
//     Core::MemoryMetricsManager::Get().Allocate(size);
//     void* pointer = std::malloc(size);
//     if (!pointer) throw std::bad_alloc();
//     return pointer;
// }
//
//
//
// void operator delete(void* pointer, std::size_t size) noexcept
// {
//     if (pointer) {
//         Core::MemoryMetricsManager::Get().Free(size);
//         std::free(pointer);
//     }
// }
//
//
// void operator delete(void* pointer) noexcept
// {
//     if (pointer) {
//         Core::MemoryMetricsManager::Get().Free(0);
//         std::free(pointer);
//     }
// }
void* operator new(std::size_t size) {
    Core::MemoryMetricsManager::Get().Allocate(size);
    return malloc(size);
}

void* operator new[](std::size_t size) {
    Core::MemoryMetricsManager::Get().Allocate(size);
    return malloc(size);
}

void* operator new(std::size_t size, std::align_val_t alignment) {
    Core::MemoryMetricsManager::Get().Allocate(size);
    return aligned_alloc(static_cast<size_t>(alignment), size);
}

void operator delete(void* ptr, std::size_t size) noexcept {

    Core::MemoryMetricsManager::Get().Free(size);
    free(ptr);
}

void operator delete[](void* ptr, std::size_t size) noexcept {
    Core::MemoryMetricsManager::Get().Free(size);
    free(ptr);
}

void operator delete(void* ptr, std::size_t size, std::align_val_t) noexcept {
    Core::MemoryMetricsManager::Get().Free(size);
    free(ptr);
}

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
