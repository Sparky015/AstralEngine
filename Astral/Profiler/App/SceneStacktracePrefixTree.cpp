/**
* @file SceneStacktracePrefixTree.cpp
* @author Andrew Fagan 
* @date 9/11/26
*/

#include "SceneStacktracePrefixTree.h"

#include <sstream>
#include <vector>

namespace Astral {

    void SceneStacktracePrefixTree::AddStacktrace(const std::string& stacktrace)
    {

        std::vector<std::string> frames = {};

        {
            std::stringstream stringstream = std::stringstream(stacktrace);
            std::string frame = "";
            getline(stringstream, frame, '\n'); // Always ignore first line which is always "Stack trace (most recent call first):"
            while (getline(stringstream, frame, '\n'))
            {
                frames.push_back(frame);
            }
        }


        StacktracePrefixNode* currentNode = &m_Root;
        for (std::string& frame : frames)
        {
            currentNode->InclusiveMemoryOperations++;
            currentNode = &currentNode->FrameToChildNode[frame];
        }
    }


    const StacktracePrefixNode& SceneStacktracePrefixTree::GetPrefixTreeRoot()
    {
        return m_Root;
    }


    void SceneStacktracePrefixTree::ClearPrefixTree()
    {
        m_Root = StacktracePrefixNode();
    }

}
