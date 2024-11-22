//
// Created by Andrew Fagan on 7/18/24.
//

#include "EngineDebugInfoLogger.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Debug {

    void LogEngineDebugInfo()
    {
        PROFILE_SCOPE();
        LOG("\n      Engine Info/Tools/Libraries:");

        LOG("Compiled on " __DATE__ << " " << __TIME__);


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
        #elif __BORLANDC__
            LOG("Compiler: Borland C++ " << __BORLANDC__);
        #else
            LOG("Compiler: Unknown");
        #endif

        // Outputs the standard library name and version
        #if __GLIBCXX__
            LOG("Standard Library: GNU libstdc++ " << __GLIBCXX__);
        #elif _LIBCPP_VERSION
        LOG("Standard Library: LLVM libc++ " << _LIBCPP_VERSION / 1000 << '.'
                                                       << (_LIBCPP_VERSION % 1000) / 10 << '.'
                                                       << _LIBCPP_VERSION % 10);
        #elif _MSC_VER
            LOG("Standard Library: Microsoft STL " << _MSC_VER);
        #else
            LOG("Standard Library: Unknown");
        #endif

        /** Outputs the libraries used depending on the platform. */
        #ifdef PLATFORM_MACOS
            LOG("Window: GLFW " << glfwGetVersionString());
            LOG("Rendering API: OpenGL " << glGetString(GL_VERSION));
            LOG("Graphics Vendor: " << glGetString(GL_VENDOR));
            LOG("Graphics Processor: " << glGetString(GL_RENDERER));

        #elif PLATFORM_WINDOWS
            LOG("Window: GLFW " << glfwGetVersionString());
            LOG("Rendering API: OpenGL " << glGetString(GL_VERSION));
            LOG("Graphics Vendor: " << glGetString(GL_VENDOR));
            LOG("Graphics Processor: " << glGetString(GL_RENDERER));
        #endif

        LOG(""); // Creates an extra new line
    }

}