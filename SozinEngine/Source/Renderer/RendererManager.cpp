//
// Created by Andrew Fagan on 11/28/24.
//

#include "RendererManager.h"
#include "RendererCommands.h"
#include "Shaders/ShaderSource.h"

#include "glad/glad.h" //TEMP
#include "Input/InputState.h" // TEMP
#include "Window/WindowManager.h" // TEMP
#include "glm/ext/matrix_transform.hpp"

#include "ECS/ECSManager.h" // TEMP
#include "Input/Conversions.h"
#include "Game/Board/BoardManager.h"

namespace Renderer {


    RendererManager& g_RendererManager = RendererManager::Get();

    RendererManager& RendererManager::Get()
    {
        static RendererManager s_RendererManager;
        return s_RendererManager;
    }


    void RendererManager::Init()
    {
        PROFILE_SCOPE();

        TRACE("Initializing Renderer Manager!");
        m_UpdateListener.StartListening();

    }


    void RendererManager::Update()
    {
        PROFILE_SCOPE();
    }


    void RendererManager::Shutdown()
    {
        PROFILE_SCOPE();

        TRACE("Shutting down Renderer Manager!")
        m_UpdateListener.StopListening();

        // TODO: Fix this situation where member variable destructors are actually called.

//        RendererManager::~RendererManager();
    }


    RendererManager::RendererManager()
    {
        TRACE("Constructing Renderer Manager!")
    }


    RendererManager::~RendererManager()
    {
        TRACE("Destroying Renderer Manager!")
    }

}