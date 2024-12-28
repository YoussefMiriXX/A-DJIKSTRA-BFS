#include "QEdge.h"
#include "edge.h"
#include <QPainter>

QEdge::QEdge(const Edge &edge, const QColor &color, QPointF start, QPointF end)
{
    startVertexId_ = edge.getStartVertexId();
    endVertexId_ = edge.getEndVertexId();
    color_ = color;
    start_ = start;
    end_ = end;
    pen_ = QPen(QColor(140,125,35));
    pen_.setWidth(1);

}

QPainterPath QEdge::shape() const
{
    QPainterPath path;
    path.lineTo(end_);
    return path;
}

QRectF QEdge::boundingRect() const
{
    return QRectF(0,0,100,100);
}

unsigned int QEdge::getStartVertexId() const
{
    return startVertexId_;
}

unsigned int QEdge::getEndVertexId() const
{
    return endVertexId_;
}

void QEdge::setStart(const QPointF &start)
{
    start_ = start;
}

void QEdge::setEnd(const QPointF &end)
{
    end_ = end;
}

void QEdge::setPen(const QPen &pen)
{
    pen_ = pen;
}

void QEdge::paint(QPainter *painter,
           const QStyleOptionGraphicsItem *item,
           QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(item);
    painter->setPen(pen_);
    //painter->setPen(color_);
    painter->drawLine(start_*2, end_*2);
}

void QEdge::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void QEdge::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void QEdge::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    update();
}

