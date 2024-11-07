#include "Core/Engine.h"

//#include "Debug/AllocationTracker.h"

#include <sys/types.h>
#include <sys/sysctl.h>

int main()
{
    int64 numCPU;
    size_t size = sizeof(numCPU);

    // Get the number of CPU cores
    if (sysctlbyname("hw.cachelinesize", &numCPU, &size, nullptr, 0) == 0) {
        std::cout << "Size of Cache Lines: " << (float)numCPU << " bytes" << std::endl;
    } else {
        std::cerr << "Error retrieving number of CPU cores." << std::endl;
    }

    {
        Engine engine = Engine();
        engine.Run();
    }

    return 0;
}
