#ifndef QVERTEX_H
#define QVERTEX_H
#include "vertex.h"

#include <QGraphicsItem>
#include <QColor>
#include <QPen>

struct QVertex : public QGraphicsItem
{
private:
    Vertex vertex_;
    double x_;
    double y_;
    QColor color_;
    qreal radius_ = .5;
    QPointF vertexCenter_;
    double scalingFactor_ = 46;
    QPen pen_;

public:
    QVertex();
    QVertex(const Vertex &vertex, const QColor &color);

    QPainterPath shape() const override;
    QRectF boundingRect() const override;
    QPointF getVertexCenter() const;
    void setPen(const QPen &pen);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void paint(QPainter *painter,
              const QStyleOptionGraphicsItem *item,
              QWidget *widget) override;

};


#endif
