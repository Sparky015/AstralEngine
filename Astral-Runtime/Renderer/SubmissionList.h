/**
* @file SubmissionList.h
* @author Andrew Fagan
* @date 11/29/25
*/

#pragma once

#include "Asset/Ref.h"
#include "Common/Material.h"
#include "Common/Mesh.h"
#include "Core/Math/Math.h"

namespace Astral {

    class SubmissionList
    {
    public:

        void AppendSubmission(const Ref<Mesh>& mesh, const Ref<Material>& material, const Mat4& transform);
        void Reserve(int numSubmissions);
        void Resize(int numSubmissions);
        void Clear();
        size_t Size();

        void SortByMaterial(Vec3 cameraPosition);
        void SortFrontToBack(Vec3 cameraPosition);

        std::vector<Ref<Mesh>>& GetMeshes();
        std::vector<Ref<Material>>& GetMaterials();
        std::vector<Mat4>& GetTransforms();

    private:

        std::vector<Ref<Mesh>> m_Meshes;
        std::vector<Ref<Material>> m_Materials;
        std::vector<Mat4> m_Transforms;
    };

}
