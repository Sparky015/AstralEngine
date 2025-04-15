//
// Created by Andrew Fagan on 7/18/24.
//

#include "EngineDebugInfoLogger.h"

#include "Core/Engine.h"
#include "Instrumentation/ScopeProfiler.h"
#include "Utilities/Loggers.h"
#include "Window/WindowManager.h"

namespace Debug {

    void LogEngineDebugInfo()
    {
        PROFILE_SCOPE("LogEngineDebugInfo");
        LOG("\n      Engine Info/Tools/Libraries:");

        /** Outputs the build configuration of the engine. */
        #ifdef DEBUG_BUILD
        LOG("Build Configuration: Debug");
        #elif RELEASE_WITH_DEBUG_INFO_BUILD
        LOG("Build Configuration: Release with Debug Info");
        #elif MINIMUM_SIZE_RELEASE_BUILD
        LOG("Build Configuration: Minimum Size Release");
        #elif RELEASE_BUILD
        LOG("Build Configuration: Release");
        #endif

        /** Outputs the compiler name and version */
        #ifdef __clang__
        LOG("Compiler: Clang " << __clang_version__);
        #elif __GNUC__
        LOG("Compiler: GNU GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__);
        #elif _MSC_VER
        LOG("Compiler: MSVC " << _MSC_VER);
        #elif __INTEL_COMPILER
            LOG("Compiler: Intel " << __INTEL_COMPILER);
        #else
            LOG("Compiler: Unknown");
        #endif

        // Outputs the standard library name and version
        #if __GLIBCXX__
            LOG("Standard Library: GNU libstdc++ " << __GLIBCXX__);
        #elif _LIBCPP_VERSION
        LOG("Standard Library: LLVM libc++ " << _LIBCPP_VERSION / 1000 << '.'
                                                       << (_LIBCPP_VERSION % 1000) / 10 << '.'
                                                       << _LIBCPP_VERSION % 10) ;
        #elif _MSC_VER
            LOG("Standard Library: Microsoft STL " << _MSC_VER);
        #else
            LOG("Standard Library: Unknown");
        #endif

        /** Outputs the libraries used depending on the platform. */
        Window::Window& window = Engine::Get().GetWindowManager().GetWindow();
        Astral::RendererContext& rendererContext = window.GetRendererContext();

        LOG("Window: " << window.GetVersion());
        LOG("Rendering API: " << rendererContext.GetRenderingAPI());
        LOG("Graphics Vendor: " << rendererContext.GetGPUVendor());
        LOG("Graphics Processor: " << rendererContext.GetGraphicsProcessorName());


        LOG(""); // Creates an extra new line
    }

}