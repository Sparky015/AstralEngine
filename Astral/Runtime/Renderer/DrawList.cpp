/**
* @file DrawList.cpp
* @author Andrew Fagan
* @date 11/29/25
*/

#include "DrawList.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <numeric>
#include <glm/glm.hpp>

#include "Astral.h"
#include "Astral.h"
#include "glm/gtx/norm.hpp"

namespace Astral {

    void DrawList::Submit(const Ref<Mesh>& mesh, const Ref<Material>& material, const Mat4& transform)
    {
        m_Meshes.push_back(mesh);
        m_Materials.push_back(material);
        m_Transforms.push_back(transform);
    }


    void DrawList::Reserve(int numSubmissions)
    {
        m_Meshes.reserve(numSubmissions);
        m_Materials.reserve(numSubmissions);
        m_Transforms.reserve(numSubmissions);
    }


    void DrawList::Resize(int numSubmissions)
    {
        m_Meshes.resize(numSubmissions);
        m_Materials.resize(numSubmissions);
        m_Transforms.resize(numSubmissions);
    }


    void DrawList::Clear()
    {
        m_Meshes.clear();
        m_Materials.clear();
        m_Transforms.clear();
    }


    size_t DrawList::Size()
    {
        return m_Transforms.size();
    }


    void DrawList::SortByMaterial(Vec3 cameraPosition)
    {
        PROFILE_SCOPE("SortSubmissionListsByMaterial")

        std::vector<int> sortedIndices(m_Meshes.size());
        std::iota(sortedIndices.begin(), sortedIndices.end(), 0);

        std::sort(sortedIndices.begin(), sortedIndices.end(), [this, cameraPosition](int a, int b)
        {
            if (m_Materials[a].get() == m_Materials[b].get())
            {
                Vec3 positionA(m_Transforms[a][3][0], m_Transforms[a][3][1], m_Transforms[a][3][2]);
                Vec3 positionB(m_Transforms[b][3][0], m_Transforms[b][3][1], m_Transforms[b][3][2]);

                float distanceSquaredA = glm::distance2(cameraPosition, positionA);
                float distanceSquaredB = glm::distance2(cameraPosition, positionB);

                return distanceSquaredA < distanceSquaredB;
            }
            else
            {
                return m_Materials[a].get() < m_Materials[b].get();
            }
        });

        std::vector<Ref<Material>> sortedMaterials{};
        std::vector<Ref<Mesh>> sortedMeshes{};
        std::vector<Mat4> sortedTransform{};
        sortedMaterials.reserve(m_Materials.size());
        sortedMeshes.reserve(m_Meshes.size());
        sortedTransform.reserve(m_Transforms.size());

        for (int index : sortedIndices)
        {
            sortedMaterials.push_back(m_Materials[index]);
            sortedMeshes.push_back(m_Meshes[index]);
            sortedTransform.push_back(m_Transforms[index]);
        }

        m_Materials = std::move(sortedMaterials);
        m_Meshes = std::move(sortedMeshes);
        m_Transforms = std::move(sortedTransform);
    }


    void DrawList::SortFrontToBack(Vec3 cameraPosition)
    {
        PROFILE_SCOPE("SortSubmissionListsFrontToBack")

        std::vector<int> sortedIndices(m_Meshes.size());
        std::iota(sortedIndices.begin(), sortedIndices.end(), 0);

        std::sort(sortedIndices.begin(), sortedIndices.end(), [cameraPosition, this](int a, int b)
        {
            Vec3 positionA(m_Transforms[a][3][0], m_Transforms[a][3][1], m_Transforms[a][3][2]);
            Vec3 positionB(m_Transforms[b][3][0], m_Transforms[b][3][1], m_Transforms[b][3][2]);

            float distanceSquaredA = glm::distance2(cameraPosition, positionA);
            float distanceSquaredB = glm::distance2(cameraPosition, positionB);

            return distanceSquaredA < distanceSquaredB;
        });

        std::vector<Ref<Material>> sortedMaterials{};
        std::vector<Ref<Mesh>> sortedMeshes{};
        std::vector<Mat4> sortedTransform{};
        sortedMaterials.reserve(m_Materials.size());
        sortedMeshes.reserve(m_Meshes.size());
        sortedTransform.reserve(m_Transforms.size());

        for (int index : sortedIndices)
        {
            sortedMaterials.push_back(m_Materials[index]);
            sortedMeshes.push_back(m_Meshes[index]);
            sortedTransform.push_back(m_Transforms[index]);
        }

        m_Materials = std::move(sortedMaterials);
        m_Meshes = std::move(sortedMeshes);
        m_Transforms = std::move(sortedTransform);
    }


    std::vector<Ref<Mesh>>& DrawList::GetMeshes()
    {
        return m_Meshes;
    }


    std::vector<Ref<Material>>& DrawList::GetMaterials()
    {
        return m_Materials;
    }


    std::vector<Mat4>& DrawList::GetTransforms()
    {
        return m_Transforms;
    }

}
