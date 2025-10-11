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

    /**
     * @class DirectedGraph
     * @brief Defines a directed graph data structure
     * @details Uses on an adjacency list to store edges
     * @note This is not a complete graph data structure implementation. It contains just enough for building a render graph.
     */
    template <typename DataType>
    class DirectedGraph
    {
    public:

        using Vertex = Vertex<DataType>;
        using Edge = Edge<DataType>;

        /**
         * @brief Defines a directed graph data structure
         * @param vertexData The data to associate with the vertex
         * @return A vertex handle (owned by this graph)
         */
        Vertex AddVertex(DataType vertexData);

        /**
         * @brief Gets the number of vertices in the graph
         * @return The number of vertices in the graph
         */
        uint32 NumOfVertices() const { return m_Vertices.size(); }

    private:

        friend Edge;
        friend Vertex;

        std::unordered_map<Vertex, std::vector<Edge>> m_Vertices;
        std::vector<DataType> m_VerticeData;
    };


    template<typename DataType>
    typename DirectedGraph<DataType>::Vertex DirectedGraph<DataType>::AddVertex(DataType vertexData)
    {
        m_VerticeData.push_back(vertexData);
        VertexIndex vertexIndex = m_Vertices.size();
        Vertex vertex = Vertex(this, vertexIndex, vertexData);
        m_Vertices[vertex] = std::vector<Edge>();
        return vertex;
    }

}



