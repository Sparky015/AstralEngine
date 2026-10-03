/**
* @file SceneStacktracePrefixTree.h
* @author Andrew Fagan 
* @date 9/11/26
*/

#pragma once

#include <string>
#include <unordered_map>

namespace Astral {

    struct StacktracePrefixNode
    {
        size_t InclusiveMemoryOperations;
        std::unordered_map<std::string, StacktracePrefixNode> FrameToChildNode;
    };

    class SceneStacktracePrefixTree
    {
    public:

        /**
         * @brief Breakdowns a stacktrace into strings of frames and adds them to the stacktrace prefix tree
         */
        void AddStacktrace(const std::string& stacktrace);

        /**
         * @brief Gets the prefix tree's root
         * @return The prefix tree's root
         */
        const StacktracePrefixNode& GetPrefixTreeRoot();

        /**
         * @brief Clears the prefix tree to an empty state
         */
        void ClearPrefixTree();

    private:

        StacktracePrefixNode m_Root = {};
    };

}
