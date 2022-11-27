#include <QPainter>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "fruit.h"
#include "extern.h"

Fruit::Fruit(qreal x, qreal y) {
    this->setPos(x,y);
    setZValue(3);
}

QRectF Fruit::boundingRect() const {
    QRectF ret(0,0,grid_size,grid_size);
    return ret;
}

void Fruit::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillPath(shape(), Qt::red);
    painter->restore();
}

QPainterPath Fruit::shape() const {
    QPainterPath p;
    p.addEllipse(QRectF(1,1,grid_size-2,grid_size-2));
    return p;
}

