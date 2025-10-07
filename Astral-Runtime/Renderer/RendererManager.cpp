//
// Created by Andrew Fagan on 11/28/24.
//

#include "RendererManager.h"

#include "Asset/AssetManager.h"
#include "Common/CubeLUT.h"
#include "Core/Engine.h"
#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Debug/Utilities/Loggers.h"
#include "RHI/RendererAPI.h"
#include "Scenes/Scene.h"

namespace Astral {

    RendererManager::RendererManager()
    {
        AE_TRACE("Constructing Renderer Manager!")
    }


    RendererManager::~RendererManager()
    {
        AE_TRACE("Destroying Renderer Manager!")
    }


    void RendererManager::Init()
    {
        PROFILE_SCOPE("RendererManager::Init");
        AE_TRACE("Initializing Renderer Manager!");
        m_RenderContext.reset(Astral::RenderingContext::CreateRendererContext());
        m_RenderContext->Init();

        LoadRendererAssets();
        SceneRenderer::Init();
    }


    void RendererManager::Shutdown()
    {
        PROFILE_SCOPE("RendererManager::Shutdown");
        AE_TRACE("Shutting down Renderer Manager!")

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

        // Materials
        registry.GetOrCreateAsset<Material>("Materials/MissingMaterial.astmat");

        // Meshs
        registry.GetOrCreateAsset<Mesh>("Meshes/Quad.obj");

        // Shaders
        registry.GetOrCreateAsset<Shader>("Shaders/Mesh_Position_UV.vert");
        registry.GetOrCreateAsset<Shader>("Shaders/Mesh_Position_Normals_UV.vert");
        registry.GetOrCreateAsset<Shader>("Shaders/SampleImage.frag");
        registry.GetOrCreateAsset<Shader>("Shaders/ForwardLightingPassUnpacked.frag");


        // Textures
        registry.GetOrCreateAsset<Texture>("Textures/MissingTexture.png");
        registry.GetOrCreateAsset<Texture>("Textures/SolidBlack.png");
        registry.GetOrCreateAsset<Texture>("Textures/SolidWhite.png");

        // Demo Models
        // registry.RegisterAsset<Mesh>("Models/WaterBendingGauntlet.fbx");
        // registry.RegisterAsset<Material>("Models/WaterBendingGauntlet.astmat");


        // Cubemap Mesh
        registry.GetOrCreateAsset<Mesh>("Meshes/Cube.obj");

    }

}
