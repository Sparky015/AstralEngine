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
    bool LoadMemoryProfileButtonComponent();

    /**@brief Pulls up a window for the user to select a file
     * @return The file path of the file that was selected by the user. */
    std::string_view SelectFile();

}
