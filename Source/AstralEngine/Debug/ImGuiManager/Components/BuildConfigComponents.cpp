//
// Created by Andrew Fagan on 1/6/25.
//

#include "BuildConfigComponents.h"

#include "imgui.h"

void Debug::BuildProfileComponent()
{
    ImGui::Text("Build Profile: %s", ENGINE_BUILD_PROFILE);
}

void Debug::CompilerInfoComponents()
{
    /** Outputs the compiler name and version */
#ifdef __clang__
    ImGui::Text("Compiler: Clang %s", __clang_version__);
#elif __GNUC__
    ImGui::Text("Compiler: GNU GCC %d.%d.%d", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#elif _MSC_VER
    ImGui::Text("Compiler: MSVC %d.%d", _MSC_VER / 100, _MSC_VER % 100);
#elif __INTEL_COMPILER
    ImGui::Text("Compiler: Intel %d", __INTEL_COMPILER);
#elif __BORLANDC__
    ImGui::Text("Compiler: Borland C++ %d", __BORLANDC__);
#else
    ImGui::Text("Compiler: Unknown");
#endif

    // Outputs the standard library name and version
#if __GLIBCXX__
    ImGui::Text("Standard Library: GNU libstdc++ %d", __GLIBCXX__);
#elif _LIBCPP_VERSION
    ImGui::Text("Standard Library: LLVM libc++ %d.%d.%d", _LIBCPP_VERSION / 1000, (_LIBCPP_VERSION % 1000) / 10, (_LIBCPP_VERSION % 10));
#elif _MSC_VER
    ImGui::Text("Standard Library: Microsoft STL %d",  _MSC_VER);
#else
    ImGui::Text("Standard Library: Unknown");
#endif
}
