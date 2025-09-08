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
        PROFILE_SCOPE("Renderer Manager Initialization");
        AE_TRACE("Initializing Renderer Manager!");
        m_RenderContext.reset(Astral::RenderingContext::CreateRendererContext());
        m_RenderContext->Init();

        LoadRendererAssets();
        SceneRenderer::Init();
    }


    void RendererManager::Shutdown()
    {
        PROFILE_SCOPE("Renderer Manager Shutdown");
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
        registry.CreateAsset<Material>("Materials/MissingMaterial.astmat");

        // Meshs
        registry.CreateAsset<Mesh>("Meshes/Quad.obj");

        // Shaders
        registry.CreateAsset<Shader>("Shaders/Mesh_Position_UV.vert");
        registry.CreateAsset<Shader>("Shaders/Mesh_Position_Normals_UV.vert");
        registry.CreateAsset<Shader>("Shaders/SampleImage.frag");
        registry.CreateAsset<Shader>("Shaders/ForwardLightingPassUnpacked.frag");


        // Textures
        registry.CreateAsset<Texture>("Textures/MissingTexture.png");
        registry.CreateAsset<Texture>("Textures/SolidBlack.png");
        registry.CreateAsset<Texture>("Textures/SolidWhite.png");

        // Demo Models
        // registry.RegisterAsset<Mesh>("Models/WaterBendingGauntlet.fbx");
        // registry.RegisterAsset<Material>("Models/WaterBendingGauntlet.astmat");


        // Cubemap Mesh
        registry.CreateAsset<Mesh>("Meshes/Cube.obj");


        registry.CreateAsset<Texture>("LUTs/acescg_to_rec709_linear_no_shaper.cube");
    }

}
