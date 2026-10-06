/**
* @file CallTreePanel.cpp
* @author Andrew Fagan 
* @date 10/6/26
*/

#include "CallTreePanel.h"

#include "Profiler/App/ProfilerApp.h"

#include <imgui.h>

namespace Astral {

    void CallTreePanel::Show()
    {
        SceneStacktracePrefixTree& stacktracePrefixTree = ProfilerApp::Get().GetSceneDataCache().GetStacktracePrefixTree();
        const StacktracePrefixNode& root = stacktracePrefixTree.GetPrefixTreeRoot();


        for (auto& [frameName, node] : root.FrameToChildNode)
        {
            std::string nameAndCount = std::string("Operations: ") + std::to_string(node.InclusiveMemoryOperations) + "  |||  " + frameName;
            if (ImGui::TreeNode(nameAndCount.c_str()))
            {
                DisplayPrefixNode(node);
                ImGui::Spacing();
                ImGui::TreePop();
            }
        }
    }


    void CallTreePanel::DisplayPrefixNode(StacktracePrefixNode prefixNode)
    {
        for (auto& [frameName, node] : prefixNode.FrameToChildNode)
        {
            std::string nameAndCount = std::string("Operations: ") + std::to_string(node.InclusiveMemoryOperations) + "  |||  " + frameName;
            if (ImGui::TreeNode(nameAndCount.c_str()))
            {
                DisplayPrefixNode(node);
                ImGui::Spacing();
                ImGui::TreePop();
            }
        }
    }

}

