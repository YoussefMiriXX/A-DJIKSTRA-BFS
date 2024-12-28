

/* The Edge struct represents a connection between two vertices, defined by a length, a start vertex, and an end vertex.
 The struct overloads the == operator to compare edges; two edges are considered equal if their start and end vertices are the same.
*/

#pragma once

// Edge struct represents a connection between two vertices in a graph
struct Edge{
private:
    double shapelen_;                // Length or weight of the edge, representing the distance or cost of travel
    unsigned int startVertexId_;     // ID of the vertex where the edge starts
    unsigned int endVertexId_;       // ID of the vertex where the edge ends

public:
    Edge();

    // Parameterized constructor to initialize an edge with start and end vertex IDs and length
    Edge(const unsigned int &startVertexId,
         const unsigned int &endVertexId,
         const double &shapelen);

    // Getter for start vertex ID
    unsigned int getStartVertexId() const;

    // Getter for end vertex ID
    unsigned int getEndVertexId() const;

    // Getter for edge length or weight
    double getEdgeLength() const;

    // Equality operator to compare two edges based on length and vertex IDs
    bool operator==(const Edge &otherEdge) const;
};
