//
// Created by Andrew Fagan on 1/2/25.
//

#pragma once


#include "Core/SystemManager.h"

namespace Debug {

    class ChessDebugManager : public SystemManager
    {
    public:
        ChessDebugManager() = default;
        ~ChessDebugManager() override = default;

        void Init() override;
        void Shutdown() override;
        void RenderImGui() override;


        ChessDebugManager(const ChessDebugManager&) = delete;
        ChessDebugManager& operator=(const ChessDebugManager&) = delete;
        ChessDebugManager(ChessDebugManager&&) = delete;
        ChessDebugManager& operator=(ChessDebugManager&&) = delete;
    };

}
