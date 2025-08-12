/**
* @file DirectedGraph.h
* @author Andrew Fagan
* @date 7/9/2025
*/


#pragma once

#include <unordered_map>

#include "Core/FixedIntegerTypes.h"
#include "Core/Utility/Hashing/Hashes.h"

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



