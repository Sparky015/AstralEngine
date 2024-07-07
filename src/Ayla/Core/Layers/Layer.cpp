//
// Created by Andrew Fagan on 4/24/24.
//

#include "Layer.h"
#include "Ayla/Core/Application.h"

namespace Ayla::Core::Layers
{

    ILayer::ILayer() : m_IsEnabled(false), m_IsInitializedInTower(false)
    {
        AY_TRACE("[Sholas] Layer: Initializing Base Class Layer\t\t");
    }


    ILayer::~ILayer()
    {
        AY_TRACE("[Sholas] Layer: Destroying Base Class Layer\t\t");
        DetachLayer();
    }


    void ILayer::OnAttach()
    {
        /** Empty definition for when layers don't need to use this function.  */
    }


    void ILayer::OnDetach()
    {
        /** Empty definition for when layers don't need to use this function.  */
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
            Application::GetLayerStack().AppendLayer(this);
            m_IsInitializedInTower = true;
            OnAttach();
            m_IsEnabled = true;
        }
    }


    void ILayer::AttachOverlay()
    {
        if (!m_IsInitializedInTower)
        {

            Application::GetLayerStack().AppendOverlay(this);
            m_IsInitializedInTower = true;
            OnAttach();
            m_IsEnabled = true;
        }
    }


    void ILayer::DetachLayer()
    {
        if (m_IsInitializedInTower)
        {
            Application::GetLayerStack().RemoveLayer(this);
            m_IsInitializedInTower = false;
            OnDetach();
            m_IsEnabled = false;
        }
    }


    void ILayer::DetachOverlay()
    {
        if (m_IsInitializedInTower)
        {
            Application::GetLayerStack().RemoveOverlay(this);
            m_IsInitializedInTower = false;
            OnDetach();
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

} // namespace Ayla::Core::Layers