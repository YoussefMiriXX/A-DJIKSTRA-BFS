#pragma once
#include <vector>
#include <map>
#include <set>
#include <float.h>
/*
 The Vertex struct represents a node in the map graph, featuring attributes for weight and estimate, both initialized to zero.
 These are essential for pathfinding algorithms like Dijkstra and A*. The struct overloads the == and != operators for comparing vertices by their unique vertexId.
 It includes static methods for sorting vertices based on weight and estimate, aiding in prioritized operations in algorithms such as A*.
 Longitude and latitude are provided in degrees from a CSV file and converted to radians for transformation to Cartesian (x, y) coordinates.
 The constant R0 indicates the Earth's radius in meters, which is used in these calculations.
 Mean longitude and latitude serve as reference points for the Cartesian transformation.
Each vertex maintains an adjacency list as a set, efficiently representing its direct neighbors on the map.
*/

using std::set;
using std::map;
using std::vector;

// Vertex structure for graph or geographical representation.
struct Vertex {
private:
    unsigned int objectId_; // Unique ID for the vertex.
    double longitude_; // Longitude in degrees.
    double latitude_; // Latitude in degrees.
    double cartesianX_; // X coordinate in Cartesian system.
    double cartesianY_; // Y coordinate in Cartesian system.
    double weight_ = 0; // Weight of the vertex.
    double estimate_ = 0; // Estimated cost from the vertex.
    set<Vertex> adjacencyList_; // List of adjacent vertices.
    const static int R0_ = 6378137; // Earth's radius in meters.

    // Converts degrees to radians.
    float angleToRad(const double &angleInDegrees) const;

public:
    Vertex(); // Default constructor.
    Vertex(unsigned int objectId, double longitude, double latitude);

    double getLongitude() const; // Getter for longitude.
    double getLatitude() const; // Getter for latitude.
    set<Vertex> &getAdjacencyList(); // Getter for adjacency list.
    double getWeight() const; // Getter for weight.
    void setWeight(const double &weight); // Setter for weight.
    unsigned int getObjectId() const; // Getter for object ID.
    double getEstimate() const; // Getter for estimate.
    void setEstimate(const double &estimate); // Setter for estimate.
    void printVertexData() const; // Prints vertex data.

    static double getLength(Vertex startVertex, Vertex endVertex); // Calculates length between vertices.
    double convertToCartesianX(const double &meanLongitude, const double &meanLatitude) const; // Converts to Cartesian X.
    double convertToCartesianY(const double &meanLatitude) const; // Converts to Cartesian Y.

    bool operator <(const Vertex &otherVertex) const;
    bool operator ==(const Vertex &otherVertex) const;
    bool operator !=(const Vertex &otherVertex) const;

    static bool strictWeightCompare(const Vertex &thisVertex, const Vertex &otherVertex); // Compare by weight.
    static bool strictEstimateCompare(const Vertex &thisVertex, const Vertex &otherVertex); // Compare by estimate.
};
