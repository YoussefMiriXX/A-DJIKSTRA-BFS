#include "QGraph.h"
#include "graph.h"
#include "QVertex.h"
#include <iostream>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
using std::cout;

QGraph::QGraph()
{
    setSceneRect(0,0, 8000, 8000);

}

QGraph::QGraph(int width, int height)
{
    this->setSceneRect(QRectF(0, 0, width, height));

}

QGraph::QGraph(Graph graph, int width, int height)
{

    setSceneRect(0,0, 2000, 2000);
    graph_ = &graph;
}


void QGraph::addVertex(const QPointF &position,
                       QVertex &qvertex)
{
    qvertex.setPos(position);
    this->addItem(&qvertex);
}

void QGraph::addEdge(QEdge &qedge)
{
    this->addItem(&qedge);
}

void QGraph::setGraph(Graph graph)
{
    graph_ = &graph;
}

void QGraph::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    update();
}

void QGraph::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    update();
}

void QGraph::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    update();
}
