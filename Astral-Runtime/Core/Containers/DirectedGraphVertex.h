/**
* @file DirectedGraphVertex.h
* @author Andrew Fagan
* @date 8/13/2025
*/


#pragma once

#include <vector>

namespace Astral {

    using VertexIndex = uint32;

    template <typename DataType>
    class DirectedGraph;

    template <typename DataType>
    struct Edge;

    template <typename DataType>
    class Vertex
    {
    public:
        explicit Vertex(DirectedGraph<DataType>* owningGraph, VertexIndex vertexIndex, DataType data) : m_OwningGraph(owningGraph), m_VertexIndex(vertexIndex), m_VertexData(data) {}

        void AddEdge(Vertex& vertex, const DataType& edgeData);


        void SetData(const DataType& data) { m_VertexData = data; m_OwningGraph->m_VerticeData[m_VertexIndex] = data; }
        const DataType& GetData() const { return m_VertexData; }
        VertexIndex GetVertexIndex() const { return m_VertexIndex; }

        typename std::vector<Edge<DataType>>::iterator begin() { return m_OwningGraph->m_Vertices[*this].begin(); }
        typename std::vector<Edge<DataType>>::iterator end() { return m_OwningGraph->m_Vertices[*this].end(); }
        typename std::vector<Edge<DataType>>::reverse_iterator rbegin() { return m_OwningGraph->m_Vertices[*this].rbegin(); }
        typename std::vector<Edge<DataType>>::reverse_iterator rend() { return m_OwningGraph->m_Vertices[*this].rend(); }

        bool operator==(const Vertex& other) const { return m_OwningGraph == other.m_OwningGraph && m_VertexIndex == other.m_VertexIndex && m_VertexData == other.m_VertexData; }

    private:

        friend Edge<DataType>;
        DirectedGraph<DataType>* m_OwningGraph;
        VertexIndex m_VertexIndex;
        DataType m_VertexData;
    };

}