/**
* @file SystemInfoComponents.cpp
* @author Andrew Fagan
* @date 1/6/2025
*/
#include "SystemInfoComponents.h"

#ifdef __APPLE__
#include <sys/sysctl.h>
#endif

#include "Renderer/RendererContext.h"
#include "Window/WindowManager.h"
#include "cpuinfo.h"
#include "imgui.h"

namespace Debug {

    // Helper function for CPU Info
    const char* GetCPUVendorName(enum cpuinfo_vendor vendor)
    {
        switch (vendor)
        {
            case cpuinfo_vendor_unknown:            return "Unknown";
            case cpuinfo_vendor_intel:              return "Intel Corporation";
            case cpuinfo_vendor_amd:                return "Advanced Micro Devices, Inc.";
            case cpuinfo_vendor_arm:                return "ARM Holdings plc";
            case cpuinfo_vendor_qualcomm:           return "Qualcomm Incorporated";
            case cpuinfo_vendor_apple:              return "Apple Inc.";
            case cpuinfo_vendor_samsung:            return "Samsung Electronics Co., Ltd.";
            case cpuinfo_vendor_nvidia:             return "Nvidia Corporation";
            case cpuinfo_vendor_mips:               return "MIPS Technologies, Inc.";
            case cpuinfo_vendor_ibm:                return "International Business Machines Corporation";
            case cpuinfo_vendor_ingenic:            return "Ingenic Semiconductor";
            case cpuinfo_vendor_via:                return "VIA Technologies, Inc.";
            case cpuinfo_vendor_cavium:             return "Cavium, Inc.";
            case cpuinfo_vendor_broadcom:           return "Broadcom, Inc.";
            case cpuinfo_vendor_apm:                return "Applied Micro Circuits Corporation";
            case cpuinfo_vendor_huawei:             return "Huawei Technologies Co., Ltd.";
            case cpuinfo_vendor_hygon:              return "Hygon";
            case cpuinfo_vendor_sifive:             return "SiFive, Inc.";
            case cpuinfo_vendor_texas_instruments:  return "Texas Instruments Inc.";
            case cpuinfo_vendor_marvell:            return "Marvell Technology Group Ltd.";
            case cpuinfo_vendor_rdc:                return "RDC Semiconductor Co., Ltd.";
            case cpuinfo_vendor_dmp:                return "DM&P Electronics Inc.";
            case cpuinfo_vendor_motorola:           return "Motorola, Inc.";
            default:                                return "Unknown Vendor";
        }
    }



    void SystemCPUNameComponent()
    {
#ifdef __APPLE__ // cpuinfo doesn't support the M-series macbooks, so a macOS specific call is made here instead
        char cpuName[256];
        size_t size = sizeof(cpuName);
        sysctlbyname("machdep.cpu.brand_string", &cpuName, &size, nullptr, 0);
        ImGui::Text("CPU Name: %s", cpuName);
#else

        ImGui::Text("CPU Name: %s", cpuinfo_get_package(0)->name);
#endif
    }

    void SystemGPUNameComponent()
    {
        Graphics::RendererContext& rendererContext = Window::g_WindowManager.GetWindow().GetRendererContext();
        ImGui::Text("GPU Name: %s", rendererContext.GetGraphicsProcessorName().c_str());
    }

    void SystemCPUVendorComponent()
    {
        ImGui::Text("CPU Vendor: %s", GetCPUVendorName(cpuinfo_get_clusters()->vendor));
    }

    void SystemGPUVendorComponent()
    {
        Graphics::RendererContext& rendererContext = Window::g_WindowManager.GetWindow().GetRendererContext();
        ImGui::Text("GPU Vendor: %s", rendererContext.GetGPUVendor().c_str());
    }

    void CPUCacheSizeComponent()
    {
        [[likely]] if (cpuinfo_get_l1i_cache(0))
            { ImGui::Text("CPU L1i Cache Size: %.3f KB", cpuinfo_get_l1i_cache(0)->size / 1000.0f); }

        [[likely]] if (cpuinfo_get_l1d_cache(0))
            { ImGui::Text("CPU L1d Cache Size: %.3f KB", cpuinfo_get_l1d_cache(0)->size / 1000.0f); }

        [[likely]] if (cpuinfo_get_l2_cache(0))
            { ImGui::Text("CPU L2 Cache Size: %.3f KB", cpuinfo_get_l2_cache(0)->size / 1000.0f); }

        if (cpuinfo_get_l3_cache(0))
            { ImGui::Text("CPU L3 Cache Size: %.3f MB", cpuinfo_get_l3_cache(0)->size / 1000000.0f); }

        [[unlikely]] if (cpuinfo_get_l4_cache(0))
            { ImGui::Text("CPU L4 Cache Size: %.3f MB", cpuinfo_get_l4_cache(0)->size / 1000000.0f); }
    }

    void CPUCoreInfoComponent()
    {
        ImGui::Text("CPU Physical Core Count: %d", cpuinfo_get_package(0)->core_count);
        ImGui::Text("CPU Hardware Thread Count: %d", cpuinfo_get_package(0)->processor_count);
    }

    void CPUCacheLineComponent()
    {
        [[likely]] if (cpuinfo_get_l2_cache(0))
            { ImGui::Text("CPU Cache Line Size: %d bytes", cpuinfo_get_l2_cache(0)->line_size); }
        else if (cpuinfo_get_l1d_cache(0))
            { ImGui::Text("CPU Cache Line Size: %d bytes", cpuinfo_get_l1d_cache(0)->line_size); }
    }

    void CPUCacheLinePerLevelComponent()
    {
        [[likely]] if (cpuinfo_get_l1i_cache(0))
            { ImGui::Text("CPU L1i Cache Line Size: %d bytes", cpuinfo_get_l1i_cache(0)->line_size); }

        [[likely]] if (cpuinfo_get_l1d_cache(0))
            { ImGui::Text("CPU L1d Cache Line Size: %d bytes", cpuinfo_get_l1d_cache(0)->line_size); }

        [[likely]] if (cpuinfo_get_l2_cache(0))
            { ImGui::Text("CPU L2 Cache Line Size: %d bytes", cpuinfo_get_l2_cache(0)->line_size); }

        if (cpuinfo_get_l3_cache(0))
            { ImGui::Text("CPU L3 Cache Line Size: %d bytes", cpuinfo_get_l3_cache(0)->line_size); }

        [[unlikely]] if (cpuinfo_get_l4_cache(0))
            { ImGui::Text("CPU L4 Cache Line Size: %d bytes", cpuinfo_get_l4_cache(0)->line_size); }

    }

}