/**
* @file MaterialEditorPanel.h
* @author Andrew Fagan
* @date 9/1/25
*/

#pragma once

#include "Asset/Asset.h"
#include "ECS/Entity.h"
#include "Renderer/Common/Material.h"

namespace Astral {

    class MaterialEditorPanel
    {
    public:
        static void Show();

    private:

        static Material m_MaterialCopy;
        static AssetID m_SelectedEntityMaterialID;
        static Entity m_SelectedEntity;
    };


}
