#include "QVertex.h"
#include <QColor>
#include <QFont>
#include <QPainter>
#include <iostream>
#include <ostream>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneMoveEvent>

using std::cout;
using std::endl;

QVertex::QVertex()
{

}

QVertex::QVertex(const Vertex &vertex, const QColor &color)
{

    color_ = QColor(140,125,35);

    setPen(QPen(color_));
    //vertex_ = vertex;
    x_ = vertex.convertToCartesianX(-77.0121,
                                     38.9128
                                     );

    y_ = vertex.convertToCartesianY(38.9128);
    vertexCenter_.setX(x_/scalingFactor_);
    vertexCenter_.setY(-y_/scalingFactor_);

}

QPainterPath QVertex::shape() const
{
    QPainterPath path;
    path.addEllipse(vertexCenter_,
                    2*radius_,
                    2*radius_);
    return path;
}

QRectF QVertex::boundingRect() const
{

    return QRectF(0, 0, 4, 4);
}

QPointF QVertex::getVertexCenter() const
{
    return vertexCenter_;
}

void QVertex::setPen(const QPen &pen)
{
    pen_ = pen;
}

void QVertex::paint(QPainter *painter,
              const QStyleOptionGraphicsItem *item,
              QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(item);
    painter->setBrush(pen_.color());
    painter->setPen(pen_);
    painter->drawEllipse(vertexCenter_,
                         2*radius_,
                         2*radius_);

}

void QVertex::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void QVertex::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void QVertex::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    update();
}
