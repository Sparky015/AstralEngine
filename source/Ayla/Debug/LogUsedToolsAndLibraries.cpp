//
// Created by Andrew Fagan on 7/18/24.
//

#include "LogUsedToolsAndLibraries.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Ayla::Debug {

    void LogUsedLibraries()
    {
        AY_LOG("\n      Engine Tools/Libraries:");

        /** Outputs the compiler name and version */
        #ifdef __clang__
            AY_LOG_SS("Compiler: " << __clang_version__);
        #elif defined(__GNUC__)
            AY_LOG_SS("Compiler: GNU GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__);
        #elif defined(_MSC_VER)
            AY_LOG_SS("Compiler: MSVC " << _MSC_VER);
        #elif defined(__INTEL_COMPILER)
            AY_LOG_SS("Compiler: Intel " << __INTEL_COMPILER);
        #elif defined(__BORLANDC__)
            AY_LOG_SS("Compiler: Borland C++ " << __BORLANDC__);
        #else
            AY_LOG("Compiler: Unknown");
        #endif


        // Outputs the standard library name and version
        #if defined(__GLIBCXX__) //__GLIBCXX__
            AY_LOG_SS("Standard Library: GNU libstdc++ " << __GLIBCXX__);
        #elif defined(_LIBCPP_VERSION)
            AY_LOG_SS("Standard Library: LLVM libc++ " << _LIBCPP_VERSION / 1000 << '.'
                                                       << (_LIBCPP_VERSION % 1000) / 10 << '.'
                                                       << _LIBCPP_VERSION % 10);
        #elif defined(_MSC_VER)
            AY_LOG_SS("Standard Library: Microsoft STL " << _MSC_VER);
        #else
            AY_LOG("Standard Library: Unknown");
        #endif

        /** Outputs the libraries used depending on the platform. */
        #ifdef AYLA_PLATFORM_MACOS
            AY_LOG_SS("Window: GLFW " << glfwGetVersionString());
            AY_LOG_SS("Rendering API: OpenGL " << glGetString(GL_VERSION));
        #elif defined(AYLA_PLATFORM_WINDOWS)
            AY_LOG_SS("Window: GLFW " << glfwGetVersionString());
            AY_LOG_SS("Rendering API: OpenGL " << glGetString(GL_VERSION));
        #endif

        AY_LOG(""); // Creates an extra new line
    }

}