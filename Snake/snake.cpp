#include <QPainter>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "snake.h"
#include "game.h"
#include "extern.h"

Snake::Snake(Game* _game):willgrow(0),direction(Stay),game(_game),head(0,0) {
    body.push_back(QPoint(0,grid_size));
    setZValue(-1);
}

QRectF Snake::boundingRect() const {
    qreal xmin,xmax,ymin,ymax;
    xmin = xmax = head.x();
    ymin = ymax = head.y();
    for(auto point: body) {
        xmin = xmin<point.x()?xmin:point.x();
        xmax = xmax>point.x()?xmax:point.x();
        ymin = ymin<point.y()?ymin:point.y();
        ymax = ymax>point.y()?ymax:point.y();
    }
    QPointF p1 = mapFromScene(xmin,ymin);
    QPointF p2 = mapFromScene(xmax,ymax);
    return QRectF(p1.x()-1,p1.y()-1,p2.x()-p1.x()+grid_size+1,p2.y()-p1.y()+grid_size+1);
}

void Snake::setDirection(Direction dir) {
    direction = dir;
}

Snake::Direction Snake::getDirection() {
    return direction;
}
void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    // 可以考虑换填色方案
    const QBrush br = QBrush(snake_color);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillPath(shape(),br);
    painter->setPen(QColor(0,0,0,100));
    painter->drawEllipse(QRectF(1,1,snake_size-1,snake_size-1));
    painter->restore();
}

QPainterPath Snake::shape() const
{
    QPainterPath p;
    p.setFillRule(Qt::WindingFill);
    p.addEllipse(QRectF(1,1,snake_size-1,snake_size-1));
    // 可以考虑换蛇的形状
    // https://doc.qt.io/qt-5.9/qpainterpath.html#addPolygon
    for(auto point: body) {
        auto item_point = mapFromScene(point);
        p.addRect(item_point.x(),item_point.y()
                  ,snake_size,snake_size);
    }
    return p;
}

void Snake::advance(int phase) {
    if(!phase) return;
    if(direction==Stay) return;
    // speed 相关调整
    game->time_step++;
    if(game->time_step%snake_speed !=0) return;

    game->addStep();

    body.push_back(head);

    bool terminate = false;
    switch (direction) {
        case Left:
            head.rx()-=grid_size;
            if(head.rx()<-20*grid_size) terminate = true;
        break;
        case Right:
            head.rx()+=grid_size;
            if(head.rx()>=20*grid_size) terminate = true;
        break;
        case Up:
            head.ry()-=grid_size;
            if(head.ry()<-20*grid_size) terminate = true;
        break;
        case Down:
            head.ry()+=grid_size;
            if(head.ry()>=20*grid_size) terminate = true;
        case Stay:
        break;
    }
    setPos(head);

    // 判断终止条件
    // 撞到墙或障碍物
    for(auto bl: game->game_blocks)
        if(this->collidesWithItem(bl)) {
            terminate = true;
            break;
        }
    if(body.contains(head)) terminate = true;
    if(terminate) {
        game->GameOver();
    }

    // food
    if(this->collidesWithItem(game->fruit)) {
        willgrow += 3;
        game->fruitEaten();
    }

    if(willgrow == 0) body.pop_front();
    else willgrow--;

}

