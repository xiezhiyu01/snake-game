#ifndef GRIDS_H
#define GRIDS_H

#include <QGraphicsItem>
#include "blocks.h"


class Grids : public QGraphicsItem
{
public:
    Grids(qreal x, qreal y, QGraphicsItem* block);
private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    QPainterPath shape() const;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
    QGraphicsItem* block;
};

#endif // GRIDS_H
