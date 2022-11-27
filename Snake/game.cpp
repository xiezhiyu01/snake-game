#include "game.h"
#include "extern.h"
#include <QDebug>
#include <QMessageBox>

Game::Game(QGraphicsScene* _scene):scene(_scene) {

    scene->installEventFilter(this);
}

void Game::addGridsAndBlocks() {

    // init blocks
    blocks = new Blocks**[42];
    for(int x=-21*grid_size,i=0;i<42;x+=grid_size,i++) {
        blocks[i] = new Blocks*[42];
        for(int y=-21*grid_size,j=0;j<42;y+=grid_size,j++) {
            blocks[i][j] = new Blocks(x,y);
            scene->addItem(blocks[i][j]);
        }
    }

    // init grids
    grids = new Grids**[40];
    for(int x=-20*grid_size,i=0;i<40;x+=grid_size,i++) {
        grids[i] = new Grids*[40];
        for(int y=-20*grid_size,j=0;j<40;y+=grid_size,j++) {
            grids[i][j] = new Grids(x,y,blocks[i+1][j+1]);
            scene->addItem(grids[i][j]);
        }
    }
}

void Game::GameInit() {

    score=0; step=0; time_step=0;
    emit(AddStep(step));
    emit(FruitEaten(score));
    game_blocks.clear();
    scene->clear();
    addGridsAndBlocks();

    for(int i=0;i<40;i++)
        for(int j=0;j<40;j++)
            grids[i][j]->setEnabled(true);
    for(int i=1;i<41;i++)
        for(int j=1;j<41;j++) {
            blocks[i][j]->setEnabled(true);
            blocks[i][j]->setVisible(false);
        }

    snake = new Snake(this);
    scene->addItem(snake);

}
bool Game::eventFilter(QObject *object, QEvent *event) {
    if(event->type()==QEvent::KeyPress) {
        processKeyPress((QKeyEvent*)event);
        return true;
    }
    else return QObject::eventFilter(object,event);
}

void Game::processKeyPress(QKeyEvent *event) {
    Snake::Direction dir = snake->getDirection();
    bool startkey = true;
    switch (event->key()) {
        case Qt::Key_Up:
            if(dir!=Snake::Down) snake->setDirection(Snake::Up);
            break;
        case Qt::Key_Down:
            if(dir!=Snake::Up&&dir!=Snake::Stay) snake->setDirection(Snake::Down);
            break;
        case Qt::Key_Left:
            if(dir!=Snake::Right) snake->setDirection(Snake::Left);
            break;
        case Qt::Key_Right:
            if(dir!=Snake::Left) snake->setDirection(Snake::Right);
            break;
        default:
            startkey = false;
            break;
    }
    if(dir == Snake::Stay && startkey)
        GameContinue();
}

void Game::GameStarts() {
    for(int i=0;i<40;i++)
        for(int j=0;j<40;j++)
            grids[i][j]->setEnabled(false);
    for(int i=1;i<=40;i++)
        for(int j=1;j<=40;j++) {
            blocks[i][j]->setEnabled(false);
            if(blocks[i][j]->isVisible())
                game_blocks.push_back(blocks[i][j]);
        }
    addRandomFruit();
}

void Game::addRandomFruit() {
    int x,y;
    while(1) {
        x = qrand()%40+1;
        y = qrand()%40+1;
        if(!blocks[x][y]->isVisible()) {
            if(!snake->collidesWithItem(blocks[x][y]))
                break;
        }
    }
    fruit = new Fruit((x-21)*grid_size,(y-21)*grid_size);
    scene->addItem(fruit);
}

void Game::fruitEaten() {
    score+=1;
    emit(FruitEaten(score));
    scene->removeItem(fruit);
    delete fruit;
    addRandomFruit();
}

void Game::addStep() {
    step+=1;
    emit(AddStep(step));
}

void Game::GameContinue() {
    TIMER->start(15);
    connect(TIMER,SIGNAL(timeout()),scene,SLOT(advance()));
}

void Game::GameOver() {
    TIMER->stop();
    disconnect(TIMER,SIGNAL(timeout()),scene,SLOT(advance()));
    emit(GameEnded());
}

void Game::GamePause() {
    TIMER->stop();
    disconnect(TIMER,SIGNAL(timeout()),scene,SLOT(advance()));
}

void Game::outputData(QTextStream& stream) {
    stream << step << " " << score << endl;
    stream << fruit->x() << " " << fruit->y() << endl;
    stream << snake->getDirection() << endl;
    stream << game_blocks.size() <<endl;
    for (auto block: game_blocks) {
        stream << block->x() << " " << block->y()<< endl;
    }
    stream << snake->head.rx() << " " << snake->head.ry() <<endl;
    stream << snake->body.size() << endl;
    for (auto b: snake->body) {
        stream << b.rx() <<" "<< b.ry()<<endl;
    }
}

void Game::inputData(QTextStream &stream) {

    stream >> step >> score ;
    emit(AddStep(step));
    emit(FruitEaten(score));

    game_blocks.clear();
    scene->clear();
    addGridsAndBlocks();

    int tmpx,tmpy;
    stream >> tmpx >> tmpy;
    fruit = new Fruit(tmpx,tmpy);
    scene->addItem(fruit);

    snake = new Snake(this);
    scene->addItem(snake);
    stream >> tmpx;
    snake->setDirection(Snake::Direction(tmpx));

    int n,x,y;
    stream >> n;
    for (int i=1;i<=n;i++) {
        stream >> tmpx >> tmpy;
        x = tmpx/grid_size + 21;
        y = tmpy/grid_size + 21;
        blocks[x][y]->setVisible(true);
        game_blocks.push_back(blocks[x][y]);
    }
    stream >> tmpx >> tmpy;
    snake->head = QPoint(tmpx,tmpy);
    snake->setPos(tmpx,tmpy);

    stream >> n;
    snake->body.clear();
    for (int i=1;i<=n;i++) {
        stream >> tmpx >> tmpy;
        snake->body.push_back(QPoint(tmpx,tmpy));
    }

    //grids and blocks
    for(int i=0;i<40;i++)
        for(int j=0;j<40;j++)
            grids[i][j]->setEnabled(false);
    for(int i=1;i<41;i++)
        for(int j=1;j<41;j++)
            blocks[i][j]->setEnabled(false);
}
