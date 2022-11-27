#ifndef BLOCKS_H
#define BLOCKS_H

#include <QGraphicsItem>


class Blocks : public QGraphicsItem
{
public:
    Blocks(qreal x, qreal y);
private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    QPainterPath shape() const;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
    bool is_wall;
};


#endif // BLOCKS_H
