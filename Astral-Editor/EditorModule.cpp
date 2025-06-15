/**
* @file EditorModule.cpp
* @author Andrew Fagan
* @date 2/28/25
*/

#include "Astral.h"
#include "Editor.h"
#include "Input/InputState.h"
#include "Core/Engine.h"
#include "Window/WindowManager.h"
#include "Debug/ImGui/ImGuiManager.h"

#include "ChessEntities.h"

namespace Astral {

    class EditorModule : public ApplicationModule
    {
    public:
        ~EditorModule() override = default;

        void Init() override
        {
            PROFILE_SCOPE("EditorModuleInit")
            TRACE("Initializing Editor")

            Astral::ImGuiManager& imguiManager = Astral::Engine::Get().GetImGuiManager();
            imguiManager.EnableViewportDockSpace();
            imguiManager.LoadImGuiConfigFile(std::string(ASTRAL_EDITOR_SOURCE_DIR) + "imgui-editor-config.ini");

            Astral::Window& window = Astral::Engine::Get().GetWindowManager().GetWindow();
            window.SetWindowName("Astral Editor");
            window.SetWindowDimensions(1600, 800);
            m_Editor.Init();

            m_Mesh = {};

            float vertices[20] = {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
                0.5f,  0.5f, 0.0f, 1.0f, 1.0f
        };

            uint32 indices[6] = { 0, 1, 2, 1, 3, 2};

            VertexBufferLayout bufferLayout = {
                {Float3, "a_Position"},
                {Float2, "a_TexCords"}
            };

            {
                PROFILE_SCOPE("Compile Shaders")
                m_VertexBuffer = VertexBuffer::CreateVertexBuffer(vertices, sizeof(vertices), bufferLayout);
                m_IndexBuffer = IndexBuffer::CreateIndexBuffer(indices, 6);
            }

            m_Mesh.VertexBuffer = m_VertexBuffer;
            m_Mesh.IndexBuffer = m_IndexBuffer;

            ChessEntities::InitEntities(m_Mesh);
        }

        void Update(const Astral::DeltaTime& deltaTime) override
        {

        }

        void Shutdown() override
        {
            PROFILE_SCOPE("EditorModuleShutdown")
            TRACE("Shutting down Editor")

            ChessEntities::DestroyEntities();
            m_Mesh = {};
            m_VertexBuffer.reset();
            m_IndexBuffer.reset();
            m_Editor.Shutdown();
        }

    private:

        Mesh m_Mesh;
        VertexBufferHandle m_VertexBuffer;
        IndexBufferHandle m_IndexBuffer;
        Editor m_Editor;
    };

}

IMPLEMENT_APPLICATION_MODULE(Astral::EditorModule);