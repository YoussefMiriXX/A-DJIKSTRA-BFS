#ifndef QEDGE_H
#define QEDGE_H

#include <QGraphicsItem>
#include "edge.h"
#include <QPen>

struct QEdge : public QGraphicsItem
{
private:
    unsigned int startVertexId_;
    unsigned int endVertexId_;
    QColor color_;
    QPointF start_;
    QPointF end_;
    QPen pen_;
public:
    QEdge() = default;
    QEdge(const Edge &edge, const QColor &color, QPointF start, QPointF end);
    QPainterPath shape() const override;
    QRectF boundingRect() const override;
    void addEdge(const Edge &edge) const;
    void setStart(const QPointF &start);
    void setEnd(const QPointF &end);
    void setPen(const QPen &qpen);
    unsigned int getStartVertexId() const;
    unsigned int getEndVertexId() const;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *item,
               QWidget *widget) override;
};


#endif
