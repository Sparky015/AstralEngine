#include "Core/Engine.h"
#include "Debug/EngineDebugInfoLogger.h"

#include "Debug/Macros/Loggers.h" // testing
#include "Core/Memory/Allocators/LinearAllocator.h"




int main()
{
    LOG("\n---   Entering main()    ---     (static and global object initialization above)\n");

    LOG("Compiled On: " << __DATE__ << " " << __TIME__ << "\n");

    using CustomOStringStream = std::basic_ostringstream<char, std::char_traits<char>, Core::StackLinearAllocator<char, 5024>>;

    Debug::Macros::LinearStringStream os;

    os << "This is a test! jhgkuyfjthdfcgjthfchgfhgfghgknguvgebuyrgvuyergfuvberguygvufyerguyerbguybvuyegruybvrbuyfguyervgbvuygrbyugfveubyvrgbuy";


    Debug::Macros::macro_LOG(os);


    {
        Engine engine = Engine();
        Debug::LogEngineDebugInfo(); // Need to set up window and rendering context before calling this.
        engine.Run();
    }

    LOG("\n---   Exiting main() ---     (static and global object destruction below)\n");
    return 0;
}
