#include <QPainter>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "blocks.h"
#include "extern.h"

Blocks::Blocks(qreal x, qreal y) {
    //setdata?
    this->setPos(x,y);
    if(x==-21*grid_size||y==-21*grid_size||x==20*grid_size||y==20*grid_size)
        is_wall=true,this->setEnabled(false),this->setVisible(true);
    else is_wall=false,this->hide();
    setZValue(2);
}

QRectF Blocks::boundingRect() const {
    QRectF ret(-1,-1,grid_size+2,grid_size+2);
    return ret;
}

void Blocks::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->save();
    painter->setBrush(Qt::darkGray);
    painter->drawRect(0,0,grid_size,grid_size);
    painter->restore();

}

QPainterPath Blocks::shape() const
{
    QPainterPath p;
    p.addRect(QRectF(1,1,grid_size-2,grid_size-2));
    return p;
}

void Blocks::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    if (event->button() == Qt::LeftButton) {
        if(this->isVisible() && !is_wall)
            this->hide();
    }
}
