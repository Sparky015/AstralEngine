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

    /**
     * @class Edge
     * @brief Defines an edge handle that can be used to manage an edge in a graph
     */
    template <typename DataType>
    struct Edge
    {
    public:
        explicit Edge(DirectedGraph<DataType>* owningGraph) : m_OwningGraph(owningGraph) {}

        /**
         * @brief Gets the left vertex of an edge (Edge: Left -> Right)
         * @return The left vertex of an edge
         */
        Vertex<DataType> GetLeftVertex() const { return Vertex(m_OwningGraph, m_LeftVertex, m_OwningGraph->m_VerticeData[m_LeftVertex]); }

        /**
         * @brief Gets the right vertex of an edge (Edge: Left -> Right)
         * @return The right vertex of an edge
         */
        Vertex<DataType> GetRightVertex() const { return Vertex(m_OwningGraph, m_RightVertex, m_OwningGraph->m_VerticeData[m_RightVertex]); }

        /**
         * @brief Sets the left vertex of an edge (Edge: Left -> Right)
         * @param vertex The vertex to assign to the left side of an edge
         */
        void SetLeftVertex(const Vertex<DataType>& vertex) { m_LeftVertex = vertex.m_VertexIndex; }

        /**
         * @brief Sets the right vertex of an edge (Edge: Left -> Right)
         * @param vertex The vertex to assign to the right side of an edge
         */
        void SetRightVertex(const Vertex<DataType>& vertex) { m_RightVertex = vertex.m_VertexIndex; }

        /**
         * @brief Gets the data assigned to an edge
         * @return The data assigned to an edge
         */
        DataType GetData() const { return m_EdgeData; }

        /**
         * @brief Assigns data to this edge
         * @param data Data to associate to this edge
         */
        void SetData(const DataType& data) { m_EdgeData = data; }

        /**
         * @brief Checks if this and another edge are the same
         * @return True if this edge and another edge instance represent the same edge in the same graph with the same data
         */
        bool operator==(const Edge& other) const { return m_OwningGraph == other.m_OwningGraph && m_LeftVertex == other.m_LeftVertex && m_RightVertex == other.m_RightVertex; }

    private:

        DirectedGraph<DataType>* m_OwningGraph; // TODO: Should use shared ptr in future
        VertexIndex m_LeftVertex{};
        VertexIndex m_RightVertex{};
        DataType m_EdgeData;
    };

}