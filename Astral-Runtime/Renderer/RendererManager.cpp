//
// Created by Andrew Fagan on 11/28/24.
//

#include "RendererManager.h"

#include "Asset/AssetManager.h"
#include "Core/Engine.h"
#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Debug/Utilities/Loggers.h"
#include "RHI/RendererAPI.h"

namespace Astral {

    RendererManager::RendererManager()
    {
        TRACE("Constructing Renderer Manager!")
    }


    RendererManager::~RendererManager()
    {
        TRACE("Destroying Renderer Manager!")
    }


    void RendererManager::Init()
    {
        PROFILE_SCOPE("Renderer Manager Initialization");
        TRACE("Initializing Renderer Manager!");
        m_RenderContext.reset(Astral::RenderingContext::CreateRendererContext());
        m_RenderContext->Init();

        SceneRenderer::Init();
        LoadRendererAssets();
    }


    void RendererManager::Shutdown()
    {
        PROFILE_SCOPE("Renderer Manager Shutdown");
        TRACE("Shutting down Renderer Manager!")

        SceneRenderer::Shutdown();
        m_RenderContext->Shutdown();
    }


    RenderingContext& RendererManager::GetContext()
    {
        return *m_RenderContext;
    }


    void RendererManager::LoadRendererAssets()
    {
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
        registry.SetAssetDirectoryPath(ASTRAL_RENDERER_ASSETS_DIR);

        // Materials
        registry.CreateAsset<Material>("Materials/MissingMaterial.astmat");

        // Meshs
        registry.CreateAsset<Mesh>("Meshes/Quad.obj");

        // Shaders
        registry.CreateAsset<Shader>("Shaders/Mesh_Position_UV.vert");
        registry.CreateAsset<Shader>("Shaders/Sample_Image.frag");

        // Textures
        registry.CreateAsset<Texture>("Textures/MissingTexture.png");
        registry.CreateAsset<Texture>("Textures/SolidBlack.png");
        registry.CreateAsset<Texture>("Textures/SolidWhite.png");

    }

}
