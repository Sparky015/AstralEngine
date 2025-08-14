/**
* @file DirectedGraphEdge.h
* @author Andrew Fagan
* @date 8/13/2025
*/


#pragma once

#include "DirectedGraphVertex.h"

namespace Astral {

    template <typename DataType>
    class DirectedGraph;

    template <typename DataType>
    struct Edge
    {
    public:
        explicit Edge(DirectedGraph<DataType>* owningGraph) : m_OwningGraph(owningGraph) {}

        Vertex<DataType> GetLeftVertex() const { return Vertex(m_OwningGraph, m_LeftVertex, m_OwningGraph->m_VerticeData[m_LeftVertex]); }
        Vertex<DataType> GetRightVertex() const { return Vertex(m_OwningGraph, m_RightVertex, m_OwningGraph->m_VerticeData[m_RightVertex]); }
        DataType GetData() const { return m_EdgeData; }

        void SetLeftVertex(const Vertex<DataType>& vertex) { m_LeftVertex = vertex.m_VertexIndex; }
        void SetRightVertex(const Vertex<DataType>& vertex) { m_RightVertex = vertex.m_VertexIndex; }
        void SetData(const DataType& data) { m_EdgeData = data; }

        bool operator==(const Edge& other) const { return m_OwningGraph == other.m_OwningGraph && m_LeftVertex == other.m_LeftVertex && m_RightVertex == other.m_RightVertex; }

    private:

        DirectedGraph<DataType>* m_OwningGraph;
        VertexIndex m_LeftVertex{};
        VertexIndex m_RightVertex{};
        DataType m_EdgeData;
    };

}