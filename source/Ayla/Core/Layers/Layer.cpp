//
// Created by Andrew Fagan on 4/24/24.
//

#include "Ayla/Core/Application.h"
#include "Layer.h"

namespace Ayla::Core::Layers {

    ILayer::ILayer() : m_IsEnabled(false), m_IsInitializedInTower(false)
    {
        AY_TRACE("[Sholas] Layer: Initializing Base Class Layer");
    }


    ILayer::~ILayer()
    {
        AY_TRACE("[Sholas] Layer: Destroying Base Class Layer");
        DetachLayer();
    }


    void ILayer::OnAttach()
    {
        /** Default definition for when layers don't need to use this function.  */
        m_IsEnabled = true;
    }


    void ILayer::OnDetach()
    {
        /** Default definition for when layers don't need to use this function.  */
        m_IsEnabled = false;
    }


    void ILayer::OnUpdate()
    {
        /** Empty definition for when layers don't need to use this function.  */
    }


    void ILayer::OnImGuiRender()
    {
        /** Empty definition for when layers don't need use this function.  */
    }


    void ILayer::OnEvent(IEvent& event)
    {
        /** Empty definition for when layers don't need to use this function.  */
    }


    void ILayer::AttachLayer()
    {
        if (!m_IsInitializedInTower)
        {
            Application& app = Application::Get();
            app.GetLayerStack().AppendLayer(this);
            m_IsInitializedInTower = true;
            OnAttach();
            m_IsEnabled = true;
        }
    }


    void ILayer::AttachOverlay()
    {
        if (!m_IsInitializedInTower)
        {
            Application& app = Application::Get();
            app.GetLayerStack().AppendOverlay(this);
            m_IsInitializedInTower = true;
            OnAttach();
            m_IsEnabled = true;
        }
    }


    void ILayer::DetachLayer()
    {
        if (m_IsInitializedInTower)
        {
            Application& app = Application::Get();
            app.GetLayerStack().RemoveLayer(this);
            m_IsInitializedInTower = false;
            m_IsEnabled = false;
        }
    }


    void ILayer::DetachOverlay()
    {
        if (m_IsInitializedInTower)
        {
            Application& app = Application::Get();
            app.GetLayerStack().RemoveOverlay(this);
            m_IsInitializedInTower = false;
            m_IsEnabled = false;
        }
    }


    bool ILayer::IsEnabled() const
    {
        return m_IsEnabled;
    }


    void ILayer::Enable()
    {
        m_IsEnabled = true;
    }


    void ILayer::Disable()
    {
        m_IsEnabled = false;
    }


    void ILayer::SetCallback(std::function<void(IEvent&)> callback)
    {
        m_Callback = std::move(callback);
    }


    void ILayer::SendEvent(IEvent& event, EventDestination eventDestination) const
    {
        if (eventDestination == FRONT_TO_BACK)
        {
            Application& app = Application::Get();
            app.GetLayerStack().DispatchEventFromFrontToBack(event);
        }
        else if (eventDestination == BACK_TO_FRONT)
        {
            Application& app = Application::Get();
            app.GetLayerStack().DispatchEventBackToFront(event);
        }
        else
        {
            AY_ERROR("Core/Layer.cpp: Event destination does not exist!")
        }
    }

} // namespace Ayla::Core::Layers