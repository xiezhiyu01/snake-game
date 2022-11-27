#include <QPainter>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPen>

#include "grids.h"
#include "extern.h"

Grids::Grids(qreal x, qreal y,QGraphicsItem* _block) {
    this->setPos(x,y);
    block = _block;
    setZValue(1);
}

QRectF Grids::boundingRect() const {
    QRectF ret(-1,-1,grid_size+2,grid_size+2);
    return ret;
}

void Grids::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
      painter->save();
      QPen pen; pen.setColor(QColor(0,0,0,200));
      painter->setPen(pen);
      painter->drawRect(QRectF(0,0,grid_size,grid_size));
//    painter->fillPath(shape(), Qt::darkGray);
      painter->restore();
}

QPainterPath Grids::shape() const {
    QPainterPath p;
    p.addRect(QRectF(0,0,grid_size,grid_size));
    return p;
}

void Grids::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if(this->x()==0&&this->y()==0) return;
        if(this->x()==0&&this->y()==grid_size) return;
        if(!block->isVisible())
            block->setVisible(true);
    }
}
