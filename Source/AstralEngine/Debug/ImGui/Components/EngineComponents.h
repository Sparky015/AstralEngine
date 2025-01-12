/**
* @file EngineComponents.h
* @author Andrew Fagan
* @date 1/12/2025
*/

#pragma once

namespace Debug {

    /**@brief Creates a ImGui text line that has the time since the engine was initialized
     * (aka the amount of time the program has been running) */
    void EngineTimeComponent();

    /**@brief Creates a ImGui button that will reset the engine's clock */
    void EngineTimeResetButtonComponent();

}