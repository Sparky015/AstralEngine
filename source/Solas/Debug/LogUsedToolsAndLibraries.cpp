//
// Created by Andrew Fagan on 7/18/24.
//

#include "LogUsedToolsAndLibraries.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Solas::Debug {

    void LogUsedLibraries()
    {
        AY_LOG("\n      Engine Tools/Libraries:");

        /** Most of these will never be used but I will keep it just in case logging gets turned on for some reason. */
        #ifdef AYLA_DEBUG_BUILD
            AY_LOG("Build Configuration: Debug");
        #elif AYLA_RELEASE_WITH_DEBUG_INFO_BUILD
            AY_LOG("Build Configuration: Release with Debug Info");
        #elif AYLA_MINIMUM_SIZE_RELEASE_BUILD
            AY_LOG("Build Configuration: Minimum Size Release");
        #elif AYLA_RELEASE_BUILD
            AY_LOG("Build Configuration: Release");
        #endif

        /** Outputs the compiler name and version */
        #ifdef __clang__
            AY_LOG("Compiler: Clang " << __clang_version__);
        #elif __GNUC__
            AY_LOG("Compiler: GNU GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__);
        #elif _MSC_VER
            AY_LOG("Compiler: MSVC " << _MSC_VER);
        #elif __INTEL_COMPILER
            AY_LOG("Compiler: Intel " << __INTEL_COMPILER);
        #elif __BORLANDC__
            AY_LOG("Compiler: Borland C++ " << __BORLANDC__);
        #else
            AY_LOG("Compiler: Unknown");
        #endif

        // Outputs the standard library name and version
        #if __GLIBCXX__
            AY_LOG("Standard Library: GNU libstdc++ " << __GLIBCXX__);
        #elif _LIBCPP_VERSION
            AY_LOG("Standard Library: LLVM libc++ " << _LIBCPP_VERSION / 1000 << '.'
                                                       << (_LIBCPP_VERSION % 1000) / 10 << '.'
                                                       << _LIBCPP_VERSION % 10);
        #elif _MSC_VER
            AY_LOG("Standard Library: Microsoft STL " << _MSC_VER);
        #else
            AY_LOG("Standard Library: Unknown");
        #endif

        /** Outputs the libraries used depending on the platform. */
        #ifdef AYLA_PLATFORM_MACOS
            AY_LOG("Window: GLFW " << glfwGetVersionString());
            AY_LOG("Rendering API: OpenGL " << glGetString(GL_VERSION));
            AY_LOG("Graphics Vendor: " << glGetString(GL_VENDOR));
            AY_LOG("Graphics Processor: " << glGetString(GL_RENDERER));

        #elif AYLA_PLATFORM_WINDOWS
            AY_LOG("Window: GLFW " << glfwGetVersionString());
            AY_LOG("Rendering API: OpenGL " << glGetString(GL_VERSION));
            AY_LOG("Graphics Vendor: " << glGetString(GL_VENDOR));
            AY_LOG("Graphics Processor: " << glGetString(GL_RENDERER));


        #endif

        // make ayla defined macro for file name (__file_name__)

        AY_LOG(""); // Creates an extra new line
    }

}