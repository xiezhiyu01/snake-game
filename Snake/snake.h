#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QRectF>

const int snake_size = 15;

class Game;

class Snake : public QGraphicsItem {

public:
    Snake(Game* game);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    enum Direction{Up,Right,Down,Left,Stay};

private:
    //int speed;
    int willgrow;
    enum Direction direction = Stay;
    Game* game;
    QPoint head;
    QList<QPoint> body;

public:
    void setDirection(Direction dir);
    Direction getDirection();
    void advance(int phase);
signals:
    void addStep(int step);

    friend class Game;
};

#endif // SNAKE_H
