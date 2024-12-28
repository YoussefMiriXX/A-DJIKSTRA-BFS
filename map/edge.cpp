#include "edge.h"
#include <math.h>

Edge::Edge()
{

}
Edge::Edge(const unsigned int &startVertexId, 
    const unsigned int &endVertexId, const double &shapelen)
{   
    shapelen_ = shapelen;
    startVertexId_ = startVertexId;
    endVertexId_ = endVertexId;
}

bool Edge::operator==(const Edge &otherEdge) const
{
    return shapelen_ == otherEdge.getEdgeLength() 
            && startVertexId_ == otherEdge.getStartVertexId()
            && endVertexId_ == otherEdge.getEndVertexId();
}

unsigned int Edge::getStartVertexId() const
{
    return startVertexId_;
}

unsigned int Edge::getEndVertexId() const
{
    return endVertexId_;
}

double Edge::getEdgeLength() const
{
    if(startVertexId_ != endVertexId_)
    {
        return shapelen_;
    }
    return 0;
}


