//
// Created by Andrew Fagan on 4/24/24.
//

#include "Layer.h"
#include "Ayla/Core/Application.h"

namespace Ayla::Core::Layers
{

    ILayer::ILayer()
    {
        //AY_TRACE("[Sholas] Layer: Initializing Base Class Layer\t\t");
        m_IsEnabled = false;
        m_IsInitializedInTower = false;
    }


    ILayer::~ILayer()
    {
        AY_TRACE("[Sholas] Layer: Destroying Base Class Layer\t\t");
        DetachLayer();
    }


    void ILayer::AttachLayer()
    {
        if (!m_IsInitializedInTower){
            Application::GetLayerStack().appendLayer(this);
            m_IsInitializedInTower = true;
            OnAttach();
            m_IsEnabled = true;
        }
    }


    void ILayer::AttachOverlay()
    {
        if (!m_IsInitializedInTower){

            Application::GetLayerStack().appendOverlay(this);
            m_IsInitializedInTower = true;
            OnAttach();
            m_IsEnabled = true;
        }
    }


    void ILayer::DetachLayer()
    {
        if (m_IsInitializedInTower) {
            Application::GetLayerStack().removeLayer(this);
            m_IsInitializedInTower = false;
            OnDetach();
            m_IsEnabled = false;
        }
    }


    void ILayer::DetachOverlay() {
        if (m_IsInitializedInTower) {
            Application::GetLayerStack().removeOverlay(this);
            m_IsInitializedInTower = false;
            OnDetach();
            m_IsEnabled = false;
        }
    }


    bool ILayer::IsEnabled() {
        return m_IsEnabled;
    }


    void ILayer::Enable() {
        m_IsEnabled = true;
    }


    void ILayer::Disable() {
        m_IsEnabled = false;
    }


    void ILayer::SetCallback(std::function<void(Event&)> callback) {
        m_Callback = std::move(callback);
    }

}