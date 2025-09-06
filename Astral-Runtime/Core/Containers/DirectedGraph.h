/**
* @file DirectedGraph.h
* @author Andrew Fagan
* @date 7/9/2025
*/


#pragma once

#include <unordered_map>

#include "Core/FixedIntegerTypes.h"
#include "Core/Utility/Hashing/Hashes.h"

#include "DirectedGraphEdge.h"
#include "DirectedGraphVertex.h"

#include <vector>

namespace Astral {

    template <typename DataType>
    class DirectedGraph
    {
    public:

        using Vertex = Vertex<DataType>;
        using Edge = Edge<DataType>;

        Vertex AddVertex(DataType vertexData)
        {
            m_VerticeData.push_back(vertexData);
            VertexIndex vertexIndex = m_Vertices.size();
            Vertex vertex = Vertex(this, vertexIndex, vertexData);
            m_Vertices[vertex] = std::vector<Edge>();
            return vertex;
        }

        uint32 NumOfVertices() const { return m_Vertices.size(); }

    private:

        friend Edge;
        friend Vertex;

        std::unordered_map<Vertex, std::vector<Edge>> m_Vertices;
        std::vector<DataType> m_VerticeData;
    };


    template<typename DataType>
    void Vertex<DataType>::AddEdge(Vertex& vertex, const DataType& edgeData)
    {
        Edge edge = Edge<DataType>(m_OwningGraph);
        edge.SetLeftVertex(*this);
        edge.SetRightVertex(vertex);
        edge.SetData(edgeData);

        if (std::ranges::find(m_OwningGraph->m_Vertices[*this], edge) != std::ranges::end(m_OwningGraph->m_Vertices[*this])) { return; }

        m_OwningGraph->m_Vertices[*this].push_back(edge);
    }

}



