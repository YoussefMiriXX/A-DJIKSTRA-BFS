#include "graph.h"
#include "vertex.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#include <deque>
#include <set>
#include <algorithm>
#include <chrono>
#include "commify.h"

using std::vector;
using std::string;
using std::deque;
using std::set;
using std::cout;
using std::endl;

Graph::Graph()
{

}


Graph::Graph(std::string filename)
{
    filename_ = filename;
    storeVerticesAndEdges();
    makeConnections();

}

void Graph::storeVerticesAndEdges()
{
    /*
    Stores edges data in the edges_ vector
    Stores vertices data in the map verticesMap_ as follows : verticesMap_[vertexID] = pair(longitude, latitude)
    */
    vector<string> row;
    string line, word;
    std::ifstream file(filename_, std::ios::in);

    unsigned int vertexId;
    double vertexLongitude;
    double vertexLatitude;
    unsigned int startVertexId;
    unsigned int endVertexId;
    double length;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            row.clear();
            std::stringstream str(line);
            while(getline(str, word, ','))
            {
                row.push_back(word);
            }

            if (row[0] == "V")
            {
                vertexId = stod(row[1]);
                vertexLongitude = stod(row[2]);
                vertexLatitude = stod(row[3]);
                longitudeOfCenter_+=vertexLongitude;
                latitudeOfCenter_+=vertexLatitude;
                //verticesMap_[vertexId] = std::make_pair(vertexLongitude, vertexLatitude);
                Vertex vertex(vertexId, vertexLongitude, vertexLatitude);
                allVertices_[vertexId] = vertex;
                //vertices_.push_back(row);
            }
            else if (row[0] == "E")
            {
                startVertexId = stod(row[1]);
                endVertexId = stod(row[2]);
                length = stod(row[3]);

                //edgesMap_[startVertexId][endVertexId] = length;
                Edge edge(startVertexId, endVertexId, length);
                allEdges_[{startVertexId,endVertexId}] = edge;
                //edgesMap_[endVertexId].insert(std::pair<unsigned int, double>(startVertexId, length));
 
            }
        }
        longitudeOfCenter_ /= allVertices_.size();
        latitudeOfCenter_ /= allVertices_.size();
    }

}

void Graph::makeConnections()
{
    map<pair<unsigned int, unsigned int>, Edge>::iterator it;

    unsigned int startVertexId;
    unsigned int endVertexId;
    for(it = allEdges_.begin(); it!=allEdges_.end(); ++it)
    {
        startVertexId = it->first.first;
        endVertexId = it->first.second;
        allVertices_.at(startVertexId).getAdjacencyList().insert(allVertices_.at(endVertexId));
        allVertices_.at(endVertexId).getAdjacencyList().insert(allVertices_.at(startVertexId));
    }
}

pair<vector<Vertex>, set<Vertex>> Graph::bfs(const Vertex &startVertex, const Vertex &endVertex)
{
    //one way problem
    //vector<Vertex> activeQueue;
    const auto start{std::chrono::steady_clock::now()};

    deque<Vertex> activeQueue;
    set<Vertex> alreadyVisitedVertices;
    map<Vertex, Vertex> parents;
    Vertex currentVertex;
    bool found = false;

    activeQueue.emplace_back(startVertex); 
    alreadyVisitedVertices.insert(startVertex);

    while(!activeQueue.empty() && !found)
    {

        currentVertex = allVertices_.at(activeQueue.front().getObjectId());
        //cout << "currentVertex = " ;
        //currentVertex.printVertexData();
        activeQueue.pop_front();
        alreadyVisitedVertices.insert(currentVertex);

        for(Vertex adjacentVertex : currentVertex.getAdjacencyList())
        {
            //adjacentVertex.printVertexData();
            auto adjacentVertexId = adjacentVertex.getObjectId();
            auto currentVertexId = currentVertex.getObjectId();
            try
            {
                auto edgeStartId = allEdges_.at({currentVertexId, adjacentVertexId}).getStartVertexId();
                if (alreadyVisitedVertices.find(adjacentVertex) != alreadyVisitedVertices.end()
                || currentVertexId != edgeStartId)
                //closed set
                 //If Vertex already visited
                { 
                    continue;
                }
            }
            catch(const std::out_of_range& e)
            {
                continue;
            }

            if(std::find(activeQueue.begin(), activeQueue.end(), adjacentVertex) == activeQueue.end())
            {                
                activeQueue.push_back(adjacentVertex);
                parents[adjacentVertex] =  currentVertex;
                if(adjacentVertex == endVertex)
                {
                    found = true;
                    cout << "found ! " << endl;
                    break;
                }
                
            }
        }


    }

    unsigned int numberOfVertices = alreadyVisitedVertices.size();
    cout << "Number of vertices visited = " << numberOfVertices << endl;
    vector<Vertex> VerticesOnPath = reconstructPath(parents, startVertex, endVertex);
    int i = 1;
    double length = 0;
    //auto childId = startVertex.getObjectId();
    unsigned int childVertexId;
    unsigned int parentVertexId = startVertex.getObjectId();
    for(auto it = VerticesOnPath.rbegin(); it != VerticesOnPath.rend(); ++it)
    {   
        childVertexId = it->getObjectId();
        if (parentVertexId!=childVertexId)
        {
            length += allEdges_.at({parentVertexId, childVertexId}).getEdgeLength();
        }        
        parentVertexId = childVertexId;
        cout << "Vertex["<< i << "] = " << childVertexId << ", length = " << length <<endl;
        i++;  
    }
    cout << "Total vertices on path from start to end = " << VerticesOnPath.size() << endl;
    const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> elapsed_seconds{end - start};
    Commify c(elapsed_seconds.count()*1E6);
    cout << "Path computed in " << c << "µs using BFS" <<  endl;
    return {VerticesOnPath, alreadyVisitedVertices};
}

pair<vector<Vertex>, set<Vertex>> Graph::dijkstra(const Vertex &startVertex, const Vertex &endVertex)
{
    cout << "dijkstra" << endl;
    const auto start{std::chrono::steady_clock::now()};
    deque<Vertex> activeQueue;
    set<Vertex> alreadyVisitedVertices; //closed set
    map<Vertex, Vertex> parents;
    activeQueue.push_back(startVertex);

    Vertex currentVertex;
    unsigned int currentVertexId;
    unsigned int adjacentVertexId;
    double sumOfWeights;
    
    while(!activeQueue.empty())
    {
        currentVertex = allVertices_.at(activeQueue.front().getObjectId());
        activeQueue.pop_front();
        //currentVertex.setWeight(FLT_MAX);

        if(currentVertex == endVertex)
        {
            break;
        }

        alreadyVisitedVertices.insert(currentVertex);

        for(const Vertex &v : currentVertex.getAdjacencyList())
        {

            
            adjacentVertexId =  v.getObjectId();
            currentVertexId = currentVertex.getObjectId();
            Vertex &adjacentVertex = allVertices_.at(adjacentVertexId);

            try
            {
                auto edgeStartId = allEdges_.at({currentVertexId, adjacentVertexId}).getStartVertexId();
                if (alreadyVisitedVertices.find(adjacentVertex) != alreadyVisitedVertices.end()
                || currentVertexId != edgeStartId)

                { 
                    continue;
                }
            }
            catch(const std::out_of_range& e)
            {
                continue;
            }

            sumOfWeights = currentVertex.getWeight() + computeWeight(currentVertex, adjacentVertex);
            //sum of all Weights up until currentVertex; 

            if(std::find(activeQueue.begin(), activeQueue.end(), adjacentVertex) == activeQueue.end())
            // if adjacentVertex is not already in activeQueue
            {
                adjacentVertex.setWeight(sumOfWeights);
                activeQueue.emplace_back(adjacentVertex);
                parents[adjacentVertex] = currentVertex;
            }
            else if(sumOfWeights < adjacentVertex.getWeight())
            //if adjacentVertex is already in activeQueue and the sum of weights is lesser than its current weight
            {
                adjacentVertex.setWeight(sumOfWeights);
                parents[adjacentVertex] = currentVertex;
            }
        //cout << "weight : " << adjacentVertex.getWeight() << endl;
        }
        std::sort(activeQueue.begin(), activeQueue.end(), Vertex::strictWeightCompare);
    
    }
    unsigned int numberOfVertices = alreadyVisitedVertices.size();
    cout << "Number of vertices visited = " << numberOfVertices << endl;
    vector<Vertex> VerticesOnPath = reconstructPath(parents, startVertex, endVertex);

    int i = 1;
    double length = 0;
    //auto childId = startVertex.getObjectId();
    unsigned int childVertexId;
    unsigned int parentVertexId = startVertex.getObjectId();
    for(auto it = VerticesOnPath.rbegin(); it != VerticesOnPath.rend(); ++it)
    {   
        childVertexId = it->getObjectId();
        if (parentVertexId!=childVertexId)
        {
            length += allEdges_.at({parentVertexId, childVertexId}).getEdgeLength();
        }
        parentVertexId = childVertexId;
        cout << "Vertex["<< i << "] = " << childVertexId << ", length = " << length <<endl;
        i++;  
    }
    cout << "Total vertices on path from start to end = " << VerticesOnPath.size() << endl;
    cout << "length = " << length << endl;
    const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> elapsed_seconds{end - start};
    Commify c(elapsed_seconds.count()*1E6);
    cout << "Path computed in " << c << "µs using DIJKSTRA" <<  endl;
    return {VerticesOnPath, alreadyVisitedVertices};
}

pair<vector<Vertex>, set<Vertex>> Graph::astar(const Vertex &startVertex, const Vertex &endVertex)
{
    cout << "astar" << endl;
    const auto start{std::chrono::steady_clock::now()};

    deque<Vertex> activeQueue;
    set<Vertex> alreadyVisitedVertices; //closed set
    map<Vertex, Vertex> parents;
    activeQueue.emplace_back(startVertex);
    unsigned int currentVertexId;
    unsigned int adjacentVertexId;


    while(!activeQueue.empty())
    {
        Vertex &currentVertex = allVertices_.at(activeQueue.front().getObjectId());
        activeQueue.pop_front();


        if(currentVertex == endVertex)
        {
            break;
        }

        alreadyVisitedVertices.insert(currentVertex);

        for(Vertex v : currentVertex.getAdjacencyList())
        {

            //cout <<"size = " << adjacentVertex.getAdjacencyList().size() << endl;
            adjacentVertexId = v.getObjectId();
            currentVertexId = currentVertex.getObjectId();
            Vertex &adjacentVertex = allVertices_.at(adjacentVertexId);

            try
            {
                auto edgeStartId = allEdges_.at({currentVertexId, adjacentVertexId}).getStartVertexId();
                if (alreadyVisitedVertices.find(adjacentVertex) != alreadyVisitedVertices.end() || currentVertexId != edgeStartId)
                //closed set
                //If Vertex already visited
                {
                    continue;
                }
            }
            catch(const std::out_of_range& e)
            {
                continue;
            }

            auto g = currentVertex.getWeight() + allEdges_.at({currentVertexId, adjacentVertexId}).getEdgeLength();
            auto f = g + computeWeight(adjacentVertex, endVertex); //heuristic distance = computeWeight

            if(std::find(activeQueue.begin(), activeQueue.end(), adjacentVertex) == activeQueue.end() || f < adjacentVertex.getEstimate())
            // if adjacentVertex is not already in activeQueue
            {
                adjacentVertex.setWeight(g);
                adjacentVertex.setEstimate(f);
                parents[adjacentVertex] = currentVertex;
                activeQueue.emplace_back(adjacentVertex);
            }

        }
        std::sort(activeQueue.begin(), activeQueue.end(), Vertex::strictEstimateCompare);
    }
    unsigned int numberOfVertices = alreadyVisitedVertices.size();
    cout << "Number of vertices visited = " << numberOfVertices << endl;
    vector<Vertex> VerticesOnPath = reconstructPath(parents, startVertex, endVertex);

    int i = 1;
    double length = 0;
    unsigned int childVertexId;
    unsigned int parentVertexId = startVertex.getObjectId();
    for(auto it = VerticesOnPath.rbegin(); it != VerticesOnPath.rend(); ++it)

    {   
        childVertexId = it->getObjectId();
        if(parentVertexId != childVertexId)
        {
            length += allEdges_.at({parentVertexId, childVertexId}).getEdgeLength();
        }
        parentVertexId = childVertexId;
        cout << "Vertex["<< i << "] = " << childVertexId << ", length = " << length << endl;
        i++;  
    }
    cout << "Total vertices on path from start to end = " << VerticesOnPath.size() << endl;
    cout << "length = " << length << endl;

    const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> elapsed_seconds{end - start};
    Commify c(elapsed_seconds.count()*1E6);
    cout << "Path computed in " << c << "µs using ASTAR" <<  endl;
    return {VerticesOnPath, alreadyVisitedVertices};
}

vector<Vertex> Graph::reconstructPath(const map<Vertex, Vertex> &parents, 
                                    const Vertex &startVertex, 
                                    const Vertex &endVertex) const
{
    //Reconstructs the path from end to start


    vector<Vertex> vertices;
    vertices.emplace_back(endVertex);
    Vertex parent = parents.at(endVertex);
    vertices.emplace_back(allVertices_.at(parent.getObjectId()));
    Vertex oldParent;
    while(parent != startVertex)
    {
        oldParent = parent;
        parent = parents.at(parent);
        vertices.emplace_back(allVertices_.at(parent.getObjectId()));

    }
    return vertices;  
}

double Graph::computeWeight(const Vertex &startVertex, const Vertex &endVertex)
{
    /*
    Returns the euclidian distance between 2 vertices if map<startVertexId, endVertexId> is not found.
    Otherwise, returns the edge length already written in the CSV
    */
    unsigned int startVertexId = startVertex.getObjectId();
    unsigned int endVertexId = endVertex.getObjectId();
    if (allEdges_.find({startVertexId, endVertexId}) == allEdges_.end())
    {
        double cartesianStartVertexX = startVertex.convertToCartesianX(longitudeOfCenter_, latitudeOfCenter_);
        double cartesianStartVertexY = startVertex.convertToCartesianY(latitudeOfCenter_);
        double cartesianEndVertexX = endVertex.convertToCartesianX(longitudeOfCenter_, latitudeOfCenter_);
        double cartesianEndVertexY = endVertex.convertToCartesianY(latitudeOfCenter_);



        return sqrt(pow(cartesianStartVertexX - cartesianEndVertexX, 2) 
                + pow(cartesianStartVertexY - cartesianEndVertexY, 2)
                    ); 
    }

    return allEdges_.at({startVertexId, endVertexId}).getEdgeLength();    
}

map<unsigned int, Vertex> &Graph:: getAllVertices()
{
    return allVertices_;
}

map<pair<unsigned int, unsigned int>, Edge> &Graph::getAllEdges()
{
    return allEdges_;
}

double Graph::getMeanLatitude() const
{
    return latitudeOfCenter_;
}

double Graph::getMeanLongitude() const
{
    return longitudeOfCenter_;
}



