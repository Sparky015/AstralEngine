//
// Created by Andrew Fagan on 7/18/24.
//

#include "EngineDebugInfoLogger.h"

#include "Core/Engine.h"
#include "Instrumentation/ScopeProfiler.h"
#include "Utilities/Loggers.h"
#include "Renderer/RendererManager.h"
#include "Window/WindowManager.h"

namespace Astral {

    void LogEngineDebugInfo()
    {
        PROFILE_SCOPE("LogEngineDebugInfo");
        AE_LOG("\n      Engine Info/Tools/Libraries:");

        /** Outputs the build configuration of the engine. */
        #ifdef DEBUG_BUILD
        AE_LOG("Build Configuration: Debug");
        #elif RELEASE_WITH_DEBUG_INFO_BUILD
        AE_LOG("Build Configuration: Release with Debug Info");
        #elif MINIMUM_SIZE_RELEASE_BUILD
        AE_LOG("Build Configuration: Minimum Size Release");
        #elif RELEASE_BUILD
        AE_LOG("Build Configuration: Release");
        #endif

        /** Outputs the compiler name and version */
        #ifdef __apple_build_version__ // Check this before Clang because AppleClang also defines __clang__
        AE_LOG("Compiler: AppleClang " << __clang_version__); // <-- Using __clang_version__ because it is formatted nicer and is correct version number
        #elif __clang__
        AE_LOG("Compiler: Clang " << __clang_version__);
        #elif __GNUC__
        AE_LOG("Compiler: GNU GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__);
        #elif _MSC_VER
        AE_LOG("Compiler: MSVC " << _MSC_VER);
        #elif __INTEL_COMPILER
            AE_LOG("Compiler: Intel " << __INTEL_COMPILER);
        #else
            AE_LOG("Compiler: Unknown");
        #endif

        // Outputs the standard library name and version
        #if __GLIBCXX__
            AE_LOG("Standard Library: GNU libstdc++ " << __GLIBCXX__);
        #elif _LIBCPP_VERSION
        AE_LOG("Standard Library: LLVM libc++ " << _LIBCPP_VERSION / 1000 << '.'
                                                       << (_LIBCPP_VERSION % 1000) / 10 << '.'
                                                       << _LIBCPP_VERSION % 10) ;
        #elif _MSC_VER
            AE_LOG("Standard Library: Microsoft STL " << _MSC_VER);
        #else
            AE_LOG("Standard Library: Unknown");
        #endif

        /** Outputs the libraries used depending on the platform. */
        Astral::Window& window = Astral::Engine::Get().GetWindowManager().GetWindow();
        Astral::RenderingContext& renderingContext = Astral::Engine::Get().GetRendererManager().GetContext();

        AE_LOG("Window: " << window.GetVersion());
        AE_LOG("Rendering API: " << renderingContext.GetRenderingAPI());
        AE_LOG("Graphics Vendor: " << renderingContext.GetGPUVendor());
        AE_LOG("Graphics Processor: " << renderingContext.GetGraphicsProcessorName());


        AE_LOG(""); // Creates an extra new line
    }

}