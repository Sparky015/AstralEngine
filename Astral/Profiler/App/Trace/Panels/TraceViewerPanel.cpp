/**
* @file TraceViewerPanel.cpp
* @author Andrew Fagan 
* @date 10/13/26
*/

#include "TraceViewerPanel.h"

#include "Core/Utilities/Loggers.h"
#include "Profiler/App/ProfilerApp.h"

#include "imgui/imgui.h"
#include "nfd.h"

namespace Astral {

    void TraceViewerPanel::Show()
    {
        ImGui::Begin("Trace Viewer", nullptr); //, ImGuiWindowFlags_NoScrollWithMouse);

        TraceViewDataCache& traceCache = ProfilerApp::Get().GetTraceCache();

        ImGui::PushItemWidth(-FLT_MIN);
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        // static bool p = true;
        // ImGui::ShowDemoWindow(&p);


        if (!traceCache.IsCacheValid())
        {
            ImVec2 screenPosition = ImGui::GetCursorScreenPos();
            ImVec2 midWindowPosition = ImVec2(screenPosition.x + (ImGui::GetWindowWidth() / 2) - ImGui::GetFrameHeight(), screenPosition.y + (ImGui::GetWindowHeight() / 2) - ImGui::GetFrameHeight());

            ImGui::SetCursorScreenPos(midWindowPosition);

            if (ImGui::Button("Load Trace"))
            {
                nfdu8char_t* outPath;
                nfdu8filteritem_t filters[1] = { { "json", "json" }};
                nfdopendialogu8args_t args = {0};
                args.filterList = filters;
                args.filterCount = 1;

                nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

                if (result == NFD_OKAY)
                {
                    std::string outFilePath = std::string(outPath);
                    NFD_FreePathU8(outPath);
                    ProfilerApp::Get().GetTraceCache().InvalidateCache();
                    ProfilerApp::Get().GetTraceCache().InitializeCache(outFilePath);
                }
                else if (result == NFD_CANCEL)
                {
                    AE_LOG("Open File Dialog Canceled")
                }
                else if (result == NFD_ERROR)
                {
                    AE_WARN("NFD Error: " << NFD_GetError())
                }
            }

            ImGui::End();
            return;
        }

        Vec2 plotLimits = ProfilerApp::Get().GetSceneDataCache().GetViewConfig().GetPlotLimitsFilter();
        uint64 plotLimitX = (uint64)plotLimits.x + ProfilerApp::Get().GetSceneDataCache().GetSceneStartingTimePoint();
        uint64 plotLimitY = (uint64)plotLimits.y + ProfilerApp::Get().GetSceneDataCache().GetSceneStartingTimePoint();
        traceCache.SetTraceTimestampBounds({plotLimitX, plotLimitY});

        std::vector<ScopeTrace>& scopeTraces = traceCache.GetScopeTraces();

        uint64 minTimestamp = traceCache.GetMinTimestamp();
        uint64 maxTimestamp = traceCache.GetMaxTimestamp();

        uint64 minBound = traceCache.GetTraceTimestampBounds().first;
        uint64 maxBound = traceCache.GetTraceTimestampBounds().second;


        std::stack<uint64> overlappingDurations;

        // if (ImGui::IsWindowHovered())
        // {
        //     std::pair<uint64, uint64> bounds = traceCache.GetTraceTimestampBounds();
        //     uint64 boundSize = bounds.second - bounds.first;
        //     uint64 scaledStep = boundSize / 50;
        //
        //     float normalizedMousePosition = (ImGui::GetIO().MousePos.x - ImGui::GetCursorScreenPos().x) / ImGui::GetWindowWidth();
        //     constexpr float maxScrollY = 2'000'000.0f;
        //     float scrollYScaled = std::min(ImGui::GetIO().MouseWheel * (float)scaledStep, maxScrollY) * 2;
        //     bounds.first += (int64)(normalizedMousePosition * scrollYScaled);
        //     bounds.second -= (int64)((1 - normalizedMousePosition) * scrollYScaled);
        //
        //     if (ImGui::IsKeyDown(ImGuiKey_MouseLeft))
        //     {
        //         int64 mousePanScaledStep = (scaledStep * ImGui::GetIO().MouseDelta.x * -1) / 7; // Divided by 7 to make panning not so fast
        //         if (bounds.first + mousePanScaledStep > minTimestamp && bounds.second + mousePanScaledStep < maxTimestamp)
        //         {
        //             bounds.first += mousePanScaledStep;
        //             bounds.second += mousePanScaledStep;
        //         }
        //     }
        //     if (ImGui::IsKeyDown(ImGuiKey_LeftArrow))
        //     {
        //         if (bounds.first - scaledStep > minTimestamp)
        //         {
        //             bounds.first -= scaledStep;
        //             bounds.second -= scaledStep;
        //         }
        //     }
        //     if (ImGui::IsKeyDown(ImGuiKey_RightArrow))
        //     {
        //         if (bounds.second + scaledStep < maxTimestamp)
        //         {
        //             bounds.first += scaledStep;
        //             bounds.second += scaledStep;
        //         }
        //     }
        //
        //     bounds.first = std::max(bounds.first, traceCache.GetMinTimestamp());
        //     bounds.second = std::min(bounds.second, traceCache.GetMaxTimestamp());
        //     traceCache.SetTraceTimestampBounds(bounds);
        // }


        float barHeight = ImGui::GetFrameHeight();
        ImVec2 baseScreenPosition = ImGui::GetCursorScreenPos();

        uint32 currentThreadNumber = 0;

        ImGui::Text("Thread 0");
        baseScreenPosition.y += barHeight * 1.25;

        for (const ScopeTrace& scopeTrace : scopeTraces)
        {
            if (scopeTrace.Timestamp > maxBound || scopeTrace.Timestamp + scopeTrace.Duration < minBound) { continue; }
            if (scopeTrace.Duration == 0) { continue; }

            if (scopeTrace.ThreadNumber != currentThreadNumber)
            {
                for (uint32 i = currentThreadNumber; i < scopeTrace.ThreadNumber; i++)
                {
                    if (traceCache.GetThreadMaxOverlappingDurations().contains(i))
                    {
                        uint32 currentThreadMaxOverlappingDurations = traceCache.GetThreadMaxOverlappingDurations().at(i);
                        baseScreenPosition.y += barHeight * (currentThreadMaxOverlappingDurations + 1);

                        ImGui::SetCursorScreenPos(baseScreenPosition);
                        ImGui::Text("Thread %u", i + 1);
                        baseScreenPosition.y += barHeight * 1.5;
                    }
                }

                overlappingDurations = {};
                currentThreadNumber = scopeTrace.ThreadNumber;
            }

            uint64 boundedTimestamp = std::clamp(scopeTrace.Timestamp, minBound, maxBound);
            uint64 boundedDuration = std::min(scopeTrace.Duration - (boundedTimestamp - scopeTrace.Timestamp), maxBound - minBound);
            float normalizedTimestampStartPosition = (float)(boundedTimestamp - minBound) / (float)(maxBound - minBound); // [0-1] range
            float normalizedTimestampEndPosition = (float)(boundedTimestamp + boundedDuration - minBound) / (float)(maxBound - minBound); // [0-1] range
            normalizedTimestampStartPosition = std::max(normalizedTimestampStartPosition, 0.0f);
            normalizedTimestampEndPosition = std::min(normalizedTimestampEndPosition, 1.0f);

            float barLocalStartPosition = normalizedTimestampStartPosition * ImGui::GetWindowWidth();
            float barLocalEndPosition = normalizedTimestampEndPosition *  ImGui::GetWindowWidth();

            while (!overlappingDurations.empty() && overlappingDurations.top() <= scopeTrace.Timestamp)
            {
                overlappingDurations.pop();
            }

            ImVec2 startPosition = ImVec2(baseScreenPosition.x + barLocalStartPosition, baseScreenPosition.y + barHeight * overlappingDurations.size());
            ImGui::SetCursorScreenPos(startPosition);

            ImVec2 p0 = ImGui::GetCursorScreenPos();
            ImVec2 p1 = ImVec2(p0.x + (barLocalEndPosition - barLocalStartPosition), p0.y + barHeight);
            ImU32 col_a = ImGui::GetColorU32(IM_COL32(scopeTrace.Timestamp % 255, scopeTrace.Duration % 255, 255, 255));


            ImGui::PushID(scopeTrace.Timestamp);
            ImGui::PushID(scopeTrace.Duration);
            ImGui::PushID(scopeTrace.ThreadNumber);
            ImGui::PushID(scopeTrace.Name.data());

            draw_list->AddRectFilled(p0, p1, col_a, 0, ImDrawFlags_RoundCornersNone);

            if (ImGui::InvisibleButton(scopeTrace.Name.data(), ImVec2((barLocalEndPosition - barLocalStartPosition), barHeight)))
            {
                // TODO: FIX GRAPH VIEWS OVERRIDE OF PLOT LIMITS
                // ProfilerViewConfig& viewConfig = ProfilerApp::Get().GetSceneDataCache().GetViewConfig();
                // uint64 graphRelativeTime = scopeTrace.Timestamp - ProfilerApp::Get().GetSceneDataCache().GetSceneStartingTimePoint();
                // viewConfig.SetPlotLimitsFilterIn(Vec2(graphRelativeTime, graphRelativeTime + scopeTrace.Duration));
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("%s\n%.3f ms\nStart: %llu\nEnd: %llu", scopeTrace.Name.data(), (float)scopeTrace.Duration / 1'000.0f, scopeTrace.Timestamp, scopeTrace.Timestamp + scopeTrace.Duration);
                ImGui::EndTooltip();
            }

            ImGui::AlignTextToFramePadding();
            ImGui::SetCursorScreenPos(startPosition);

            if (barLocalEndPosition - barLocalStartPosition > ImGui::GetFontSize())
            {
                ImGui::PushClipRect(p0, p1, false);
                ImGui::TextColored(ImVec4(0.0f, 0.0f ,0.0f ,1.0f), " %s", scopeTrace.Name.data());
                ImGui::PopClipRect();
            }


            ImGui::PopID();
            ImGui::PopID();
            ImGui::PopID();
            ImGui::PopID();

            overlappingDurations.push(scopeTrace.Timestamp + scopeTrace.Duration);
        }

        ImGui::Dummy(ImVec2(1,1));
        ImGui::End();
    }

}
