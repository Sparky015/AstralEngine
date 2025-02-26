/**
* @file LoadFileComponent.h
* @author Andrew Fagan
* @date 2/15/25
*/

#pragma once

#include <filesystem>
#include <string_view>

namespace Core {

    /**@brief Displays a "Load File" button for loading memory profiles into a memory debug window */
    void LoadMemoryProfileButtonComponent();

    /**@brief Pulls up a window for the user to select a file
     * @param outFilePath Populates the string with the file path of the file that was selected by the user. */
    bool SelectFile(std::string& outFilePath);

}
