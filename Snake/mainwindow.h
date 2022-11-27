#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QTime>
#include <QKeyEvent>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>

#include "grids.h"
#include "blocks.h"
#include "snake.h"
#include "fruit.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Game;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent* event);
private:
    Ui::MainWindow *ui;
    enum GameStatus{Start,Running,Pause,Terminate};
    enum GameStatus gameStatus = Start;
    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsView *view;
    Game* game;
    QMediaPlayer* player;
    QMediaPlaylist* playlist;
private:
    void updateButton();
    void InitScene();
    void setButtonsEnable(bool,bool,bool,bool,bool,bool,bool);

private slots:
    void on_actionStart_triggered();
    void on_actionPause_triggered();
    void on_actionContinue_triggered();
    void on_actionRestart_triggered();
    void on_actionExit_triggered();
    void on_actionSave_triggered();
    void on_actionLoad_triggered();
    void on_actioninfo_triggered();
    void on_actionSlow_triggered();
    void on_actionMedium_triggered();
    void on_actionFast_triggered();
    void on_actionYellow_triggered();
    void on_actionGreen_triggered();
    void on_musicButton_clicked();
    void changeStatusToTerminate();
    void updateScore(int score);
    void updateStep(int step);

    friend class Snake;
};
#endif // MAINWINDOW_H
