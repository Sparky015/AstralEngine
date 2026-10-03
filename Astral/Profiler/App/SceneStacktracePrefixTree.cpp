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
                std::stringstream frameStringstream = std::stringstream(frame);
                getline(frameStringstream, frame, 'x'); // Removes the frame number - x should always be in the address of the symbol (weird inconsistency can happen where there can be two spaces in between frame number and addresses when not using x)
                getline(frameStringstream, frame, ' '); // Removes the remaining part of the address
                getline(frameStringstream, frame, ' '); // Removes the "at" between the address and the symbol name
                getline(frameStringstream, frame, '\n'); // Puts the remaining part (the symbol name and file path) into 'frame'

                frames.push_back(frame);
            }
        }


        StacktracePrefixNode* currentNode = &m_Root;
        for (int i = frames.size() - 1; i >= 0; i--)
        {
            std::string& frame = frames[i];
            currentNode = &currentNode->FrameToChildNode[frame];
            currentNode->InclusiveMemoryOperations++;
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
