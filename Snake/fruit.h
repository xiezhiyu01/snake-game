#ifndef FRUIT_H
#define FRUIT_H
#include <QGraphicsItem>

class Fruit : public QGraphicsItem
{

public:
    Fruit(qreal x, qreal y);
private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    QPainterPath shape() const;

};

#endif // FRUIT_H
