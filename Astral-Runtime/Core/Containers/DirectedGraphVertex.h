/**
* @file DirectedGraphVertex.h
* @author Andrew Fagan
* @date 8/13/2025
*/


#pragma once

#include "Core/FixedIntegerTypes.h"
#include <vector>

namespace Astral {


    using VertexIndex = uint32;

    template <typename DataType>
    class DirectedGraph;

    template <typename DataType>
    struct Edge;

    /**
     * @class Vertex
     * @brief Defines a Vertex handle that can be used to manage a vertex in a graph
     */
    template <typename DataType>
    class Vertex
    {
    public:
        explicit Vertex(DirectedGraph<DataType>* owningGraph, VertexIndex vertexIndex, DataType data) : m_OwningGraph(owningGraph), m_VertexIndex(vertexIndex), m_VertexData(data) {}

        /**
         * @brief Creates an edge between this vertex and a given vertex
         * @param vertex The other vertex to create an edge from
         * @param edgeData Any data to label an edge with
         * @note Does nothing if the other vertex is not in the same graph
         */
        void AddEdge(Vertex& vertex, const DataType& edgeData);

        /**
         * @brief Assigns custom data to a vertex
         * @param data Data to associate to this vertex
         */
        void SetData(const DataType& data) { m_VertexData = data; m_OwningGraph->m_VerticeData[m_VertexIndex] = data; }

        /**
         * @brief Gets the assigned data for this vertex
         * @return The assigned data for this fertex
         */
        const DataType& GetData() const { return m_VertexData; }

        /**
         * @brief Gets the index of where this vertex is stored in the graph
         * @return The index of where this vertex is stored in the graph
         */
        VertexIndex GetVertexIndex() const { return m_VertexIndex; }

        /**
         * @brief Checks if this and another vertex are the same
         * @return True if this vertex and another vertex instance represent the same vertex in the same graph with the same data
         */
        bool operator==(const Vertex& other) const { return m_OwningGraph == other.m_OwningGraph && m_VertexIndex == other.m_VertexIndex && m_VertexData == other.m_VertexData; }

        /**
         * @brief Returns an iterator pointing the start of an array of vertices that share an edge from this vertex to the other vertex
         * @return An iterator pointing the start of an array of vertices that share an edge from this vertex to the other vertex
         */
        typename std::vector<Edge<DataType>>::iterator begin() { return m_OwningGraph->m_Vertices[*this].begin(); }

        /**
         * @brief Returns an iterator pointing the end of an array of vertices that share an edge from this vertex to the other vertex
         * @return An iterator pointing the end of an array of vertices that share an edge from this vertex to the other vertex
         */
        typename std::vector<Edge<DataType>>::iterator end() { return m_OwningGraph->m_Vertices[*this].end(); }

        /**
         * @brief Returns an iterator pointing the end of an array of vertices that share an edge from this vertex to the other vertex
         * @return An iterator pointing the end of an array of vertices that share an edge from this vertex to the other vertex
         */
        typename std::vector<Edge<DataType>>::reverse_iterator rbegin() { return m_OwningGraph->m_Vertices[*this].rbegin(); }

        /**
         * @brief Returns an iterator pointing the start of an array of vertices that share an edge from this vertex to the other vertex
         * @return An iterator pointing the start of an array of vertices that share an edge from this vertex to the other vertex
         */
        typename std::vector<Edge<DataType>>::reverse_iterator rend() { return m_OwningGraph->m_Vertices[*this].rend(); }

    private:

        friend Edge<DataType>;
        DirectedGraph<DataType>* m_OwningGraph; // TODO: Should use shared ptr in future
        VertexIndex m_VertexIndex;
        DataType m_VertexData;
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