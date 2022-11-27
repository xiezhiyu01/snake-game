#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "snake.h"
#include "fruit.h"
#include "grids.h"
#include "blocks.h"
#include <QTimer>
#include <QTime>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QVector>
#include <QTextStream>

class Game: public QObject {
    Q_OBJECT
public:
    Game(QGraphicsScene* scene);
private:
    QGraphicsScene* scene;
    Snake* snake;
    Fruit* fruit;
    QTimer* TIMER = new QTimer;
    Grids*** grids;
    Blocks*** blocks;
    QVector<Blocks*> game_blocks;
    int score;
    int step;
    int time_step;
public:
    void outputData(QTextStream& stream);
    void inputData(QTextStream& stream);
    void GameInit();
    void GameStarts();
    void GameContinue();
    void GamePause();
    void GameOver();
    void processKeyPress(QKeyEvent* event);
private:
    bool eventFilter(QObject *object, QEvent *event);
    void fruitEaten();
    void addStep();
    void addRandomFruit();
    void addGridsAndBlocks();
signals:
    void GameEnded();
    void FruitEaten(int score);
    void AddStep(int step);

    friend class Snake;
};

#endif // GAME_H
