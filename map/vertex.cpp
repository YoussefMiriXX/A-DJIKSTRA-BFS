/* The Vertex.cpp file implements methods for converting geographic coordinates (longitude and latitude) to Cartesian coordinates (x, y)
  using the Earth's radius and a Mercator projection, which is essential for accurate distance calculations in pathfinding algorithms.
 Overloaded comparison operators (<, ==, !=) allow vertices to be compared by their objectId, supporting efficient use in sets and sorted containers.
 The weight and estimate attributes, along with their getters and setters, enable distance tracking for Dijkstra’s and A* algorithms,
 while helper functions strictWeightCompare and strictEstimateCompare facilitate sorting based on these attributes.
 The adjacency list is maintained as a set of connected vertices, providing quick access to neighboring vertices during graph traversal.
 Additionally, the printVertexData() function outputs essential vertex information, aiding in debugging.
*/


#include <math.h>
#include "vertex.h"
#include <map>
#include <iostream>
#include <set>

using std::set;
using std::map;
using std::cout;
using std::endl;

Vertex::Vertex()
{
    
}

Vertex::Vertex(unsigned int objectId, 
            double longitude, 
            double latitude)
{
    objectId_ = objectId;
    longitude_ = longitude;
    latitude_ = latitude;
}

bool Vertex::operator<(const Vertex &otherVertex) const
{
    return objectId_ < otherVertex.getObjectId();
}

bool Vertex::operator==(const Vertex &otherVertex) const
{
    return objectId_ == otherVertex.getObjectId();
}

bool Vertex::operator!=(const Vertex &otherVertex) const
{
    return objectId_ != otherVertex.getObjectId();
}

bool Vertex::strictWeightCompare(const Vertex &thisVertex, const Vertex &otherVertex)
{
    return thisVertex.getWeight() < otherVertex.getWeight(); 
}

bool Vertex::strictEstimateCompare(const Vertex &thisVertex, const Vertex &otherVertex)
{
    return thisVertex.getEstimate() < otherVertex.getEstimate();
}

double Vertex::getLongitude() const
{
    return this->longitude_;
}

double Vertex::getLatitude() const
{
    return this->latitude_;
}

unsigned int Vertex::getObjectId() const
{
    return this->objectId_;
}


double Vertex::convertToCartesianX(const double &meanLongitude, const double &meanLatitude) const
{
    return R0_*cos(angleToRad(meanLatitude))*angleToRad(longitude_ - meanLongitude);
}


double Vertex::convertToCartesianY(const double &meanLatitude) const
{
    double latitudeDifferenceInRad = angleToRad(latitude_ - meanLatitude);
    return R0_*log(
                tan((latitudeDifferenceInRad/2) + (M_PI/4))
                );
}


float Vertex::angleToRad(const double &angleInDegrees) const
{
    return angleInDegrees*M_PI/180;
}



set<Vertex> &Vertex::getAdjacencyList()
{
    return adjacencyList_;
}

double Vertex::getWeight() const
{
    return weight_;
}

void Vertex::setWeight(const double &weight)
{
    weight_ = weight;
}

double Vertex::getEstimate() const
{
    return estimate_;
}

void Vertex::setEstimate(const double &estimate)
{
    estimate_ =  estimate;
}

void Vertex::printVertexData() const
{
    cout << "ObjectId = " << objectId_
         << "      " 
         << "Longitude = " << longitude_
         << "      "
         << "Latitude = " << latitude_
         <<endl;
}

