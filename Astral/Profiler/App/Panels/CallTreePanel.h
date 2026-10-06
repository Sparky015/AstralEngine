/**
* @file CallTreePanel.h
* @author Andrew Fagan 
* @date 10/6/26
*/

#pragma once

#include "Profiler/App/SceneStacktracePrefixTree.h"

namespace Astral {

    class CallTreePanel
    {
    public:

        /**
         * @brief Renders the Call Tree ImGui panel
         */
        static void Show();

    private:

        static void DisplayPrefixNode(StacktracePrefixNode prefixNode);

    };

}
