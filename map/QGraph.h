#ifndef QGRAPH_H
#define QGRAPH_H
#include <QGraphicsScene>
#include "graph.h"
#include "QVertex.h"
#include "QEdge.h"
#include <QGraphicsSceneMouseEvent>


struct QGraph : public QGraphicsScene
{
private:
    Graph *graph_;

public:
    QGraph();
    QGraph(int width, int height);
    QGraph(Graph graph, int width, int height);
    void setGraph(Graph graph);
    void addVertex(const QPointF &position,
                         QVertex &qvertex);
    void addEdge(QEdge &edge);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

};

#endif // QGRAPH_H
