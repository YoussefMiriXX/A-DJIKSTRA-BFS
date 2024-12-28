#pragma once

#include <string>
#include <vector>
#include <map>
#include "vertex.h"
#include "edge.h"

using std::vector;
using std::string;
using std::map;
using std::pair;

// Graph struct represents a map as a graph of vertices (locations) and edges (connections)
struct Graph{
private:
    double longitudeOfCenter_ = 0; // Mean longitude of all vertices, representing the map's center in degrees
    double latitudeOfCenter_ = 0;  // Mean latitude of all vertices, representing the map's center in degrees

    string filename_;  // Name of the file containing vertex and edge data

    // Map of all vertices in the graph, with each vertex identified by its object ID
    map<unsigned int, Vertex> allVertices_;

    // Map of all edges in the graph, with each edge identified by a pair of start and end vertex IDs
    map<pair<unsigned int, unsigned int>, Edge> allEdges_;

    // Private helper function to load vertices and edges from the file
    void storeVerticesAndEdges();

    // Private helper function to establish adjacency relationships between vertices based on edges
    void makeConnections();

    // Reconstructs the path from start to end using the parents map created during pathfinding
    vector<Vertex> reconstructPath(const map<Vertex, Vertex> &parents,
                                   const Vertex &startVertex,
                                   const Vertex &endVertex) const;

public:
    Graph();

    // Constructor that initializes the graph by loading data from the specified filename
    Graph(string filename);

    // Getter for all vertices in the graph
    map<unsigned int, Vertex> &getAllVertices();

    // Getter for all edges in the graph
    map<pair<unsigned int, unsigned int>, Edge> &getAllEdges();

    // Computes the weight (distance) between two vertices, either from an existing edge or by calculation
    double computeWeight(const Vertex &startVertex, const Vertex &endVertex);

    // Getter for the mean latitude of the graph (center point)
    double getMeanLatitude() const;

    // Getter for the mean longitude of the graph (center point)
    double getMeanLongitude() const;

    // Breadth-First Search to find a path between two vertices
    pair<vector<Vertex>, set<Vertex>> bfs(const Vertex &startVertex, const Vertex &endVertex);

    // Dijkstra's algorithm to find the shortest path between two vertices, considering edge weights
    pair<vector<Vertex>, set<Vertex>> dijkstra(const Vertex &startVertex, const Vertex &endVertex);

    // A* algorithm to find the shortest path between two vertices, using both edge weights and heuristic estimates
    pair<vector<Vertex>, set<Vertex>> astar(const Vertex &startVertex, const Vertex &endVertex);
};
