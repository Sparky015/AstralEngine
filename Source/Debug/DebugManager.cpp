//
// Created by Andrew Fagan on 11/1/24.
//

#include "DebugManager.h"

#include "imgui.h"

#include "ImGuiDependencies/imgui_impl_opengl3.h"
#include "ImGuiDependencies/imgui_impl_glfw.h"

#include "Window/WindowManager.h"
#include "Game/BoardManager.h"

namespace Debug{

    DebugManager& g_DebugManager = DebugManager::Get();

    DebugManager& DebugManager::Get()
    {
        static DebugManager m_Instance = DebugManager();
        return m_Instance;
    }


    void DebugManager::Init()
    {
        TRACE("Initializing Debug Manager!")
        InitImGui();
        m_UpdateListener.StartListening();
        m_RenderImGuiListener.StartListening();
        m_KeyPressedListener.StartListening();
    }


    void DebugManager::Shutdown()
    {
        TRACE("Shutting down Debug Manager!")
        m_KeyPressedListener.StopListening();
        m_RenderImGuiListener.StopListening();
        m_UpdateListener.StopListening();
        ShutdownImGui();
    }


    void DebugManager::Update()
    {
        glClearColor(.1,.8,.5,1);
        glClear(GL_COLOR_BUFFER_BIT);
    }


    void DebugManager::RenderImGui()
    {
        ImGuiIO& io = ImGui::GetIO();
        const static ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;

        if (m_ShowDebugMenu)
        {
            ImGui::Begin("Debug Menu", &m_ShowDebugMenu);

            if (ImGui::TreeNode("Piece Locations"))
            {
                if (ImGui::BeginTable("Piece Table", 8, flags, ImVec2(200, 130)))
                {
                    Game::Board& board = Game::g_BoardManager.GetBoard();
                    for (uint8 square = Game::A8; square != Game::H1 + 1; square++)
                    {
                        ImGui::TableNextColumn();
                        ImGui::Text("%c", board.GetCharacterOfPiece(square));
                    }
                    ImGui::EndTable();
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Board Location IDs"))
            {
                if (ImGui::BeginTable("BoardLocationIDs", 8, flags, ImVec2(200, 130)))
                {
                    Game::Board& board = Game::g_BoardManager.GetBoard();
                    for (uint8 square = Game::A8; square != Game::H1 + 1; square++)
                    {
                        ImGui::TableNextColumn();
                        ImGui::Text("%d", square);
                    }
                    ImGui::EndTable();
                }
                ImGui::TreePop();
            }

            ImGui::End();
        }
    }


    float DebugManager::m_Time = 0.0f;
    void DebugManager::ImGuiBegin()
    {
        ImGuiIO& io = ImGui::GetIO();
        float time = static_cast<float>(glfwGetTime());
        io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
    }


    void DebugManager::ImGuiEnd()
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupCurrentContext);
        }
    }


    DebugManager::DebugManager() : m_KeyPressedListener([this](KeyPressedEvent e){ this->OnKeyPress(e);})
    {
        TRACE("Constructing Debug System!")
    }

    DebugManager::~DebugManager()
    {
        TRACE("Destroying Debug System!")
    }


    void DebugManager::InitImGui()
    {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO();

        Window::WindowManager& windowManager = Window::g_WindowManager;
        io.DisplaySize = ImVec2((float)windowManager.GetWidth(), (float)windowManager.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1,1);

        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(windowManager.GetNativeWindow()), true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }


    void DebugManager::ShutdownImGui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void DebugManager::OnKeyPress(KeyPressedEvent keyPressedEvent)
    {
        if (keyPressedEvent.keycode == GLFW_KEY_D)
        {
            m_ShowDebugMenu = !m_ShowDebugMenu;
        }
    }

}