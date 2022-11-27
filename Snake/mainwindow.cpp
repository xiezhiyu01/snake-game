#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>

QVector<Blocks*> game_blocks;
int grid_size = 15;
int snake_speed = 6;
QColor snake_color = Qt::yellow;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setWindowTitle(tr("GreedySnake"));
    // init ui font
    ui->setupUi(this);
    QFont font;
    font.setFamily("3ds Light");
    font.setPointSize(14);
    this->setFont(font);


    // init GraphicsView
    game = new Game(scene);
    view =ui->graphicsView;
    view->setScene(scene);
    scene->setSceneRect(-21*grid_size,-21*grid_size,42*grid_size,42*grid_size);
    connect(game,SIGNAL(GameEnded()),this,SLOT(changeStatusToTerminate()));
    connect(game,SIGNAL(FruitEaten(int)),this,SLOT(updateScore(int)));
    connect(game,SIGNAL(AddStep(int)),this,SLOT(updateStep(int)));

    player = new QMediaPlayer;
    playlist = new QMediaPlaylist;
    playlist->addMedia(QUrl(QDir::currentPath()+"/music.wav"));
    playlist->setCurrentIndex(1);
    playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    player->setPlaylist(playlist);
    QIcon icon; icon.addFile(":/icon/mute");
    ui->musicButton->setIcon(icon);

    // init gamestatus
    on_actionRestart_triggered();

}


void MainWindow::updateScore(int score) {
    ui->score->display(score);
}

void MainWindow::updateStep(int step) {
    ui->timer->display(step);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeStatusToTerminate() {
    gameStatus = Terminate;
    updateButton();
}

void MainWindow::setButtonsEnable(bool b1,bool b2,bool b3,bool b4,bool b5,bool b6,bool b7) {

    ui->actionSave->setEnabled(b1);
    ui->actionPause->setEnabled(b2);
    ui->actionRestart->setEnabled(b3);
    ui->actionContinue->setEnabled(b4);
    ui->actionExit->setEnabled(b5);
    ui->actionStart->setEnabled(b6);
    ui->actionLoad->setEnabled(b7);
    ui->SaveButton->setEnabled(b1);
    ui->PauseButton->setEnabled(b2);
    ui->RestartButton->setEnabled(b3);
    ui->ContinueButton->setEnabled(b4);
    ui->ExitButton->setEnabled(b5);
    ui->StartButton->setEnabled(b6);
    ui->LoadButton->setEnabled(b7);
}

void MainWindow::updateButton() {
    switch (gameStatus) {
        case Start:
            setButtonsEnable(0,0,0,0,1,1,1);
            break;

        case Running:
            setButtonsEnable(0,1,0,0,1,0,0);
            break;

        case Pause:
            setButtonsEnable(1,0,1,1,1,0,0);
            break;

        case Terminate:
            setButtonsEnable(0,0,1,0,1,0,0);
            break;
    }
    // load之后要set所有的blocks和grid

}

void MainWindow::on_actionStart_triggered() {
    //qDebug()<<"actionstart"<<endl;
    gameStatus = Running;
    updateButton();
    game->GameStarts();
}

void MainWindow::on_actionPause_triggered() {
    gameStatus = Pause;
    updateButton();
    game->GamePause();
}

void MainWindow::on_actionContinue_triggered() {
    gameStatus = Running;
    updateButton();
    game->GameContinue();

}

void MainWindow::on_actionRestart_triggered() {
    gameStatus = Start;
    updateButton();
    game->GameInit();
}

void MainWindow::on_actionExit_triggered() {

    this->close();
}

void MainWindow::on_actionSave_triggered() {

    QString fileName = QFileDialog::getSaveFileName(this,tr("Save Game"),".",tr("GameData(*.dat)"));
    QFile file(fileName);
    file.open( QIODevice::ReadWrite | QIODevice::Text );
    file.close();
    file.open( QIODevice::ReadWrite | QIODevice::Text );
    QTextStream stream(&file);
    game->outputData(stream);
    stream.flush();
    file.close();

}

void MainWindow::on_actionLoad_triggered() {

    QString path = QFileDialog::getOpenFileName(this,tr("Choose Game File To Load"),
                                                ".",tr("GameData(*.dat)"));
    if(!path.isEmpty()) {

        QFile file(path);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this,tr("Read File"),tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream stream(&file);
        game->inputData(stream);
        file.close();
        on_actionPause_triggered();
    }

}

void MainWindow::on_actioninfo_triggered() {
    QString mes;
    mes+="Welcome!\n";
    mes+="In the game, please press direction key to control the snake's movement.\n";
    mes+="However, you can't move backward.\n";
    mes+="So when the game starts, press any direction key except 'Down' to get moving.\n";
    mes+="Eat fruit to earn scores and grow longer.\n";
    mes+="Before starting the game, you can click any blank grid to turn it into a block. \n";
    mes+="If the snake hits the blocks or eats itself, the game ends.\n ";
    mes+="Use the buttons or shortcuts to Start/Pause/Continue/Restart/Save/Load/Exit. \n";
    mes+="Music:  Gun’s & Roses   by: Paradise Lunch \n";
    mes+="Powered by : xzy \n";
    QMessageBox::information(this,tr("Snake Game Info"),mes);
}
void MainWindow::closeEvent(QCloseEvent *event) {
    if(gameStatus==Running) on_actionPause_triggered();
    int ret = QMessageBox::warning(this,tr("Exit"),tr("确认关闭游戏？"), QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            event->accept(); //确认关闭
        } else {
            event->ignore(); //不关闭
        }
}
void MainWindow::on_actionSlow_triggered() {
    snake_speed=9;
}
void MainWindow::on_actionMedium_triggered() {
    snake_speed=6;
}
void MainWindow::on_actionFast_triggered() {
    snake_speed=3;
}
void MainWindow::on_actionYellow_triggered() {
    snake_color = Qt::yellow;
    scene->update();
}
void MainWindow::on_actionGreen_triggered() {
    snake_color = QColor(40,134,38);
    scene->update();
}
void MainWindow::on_musicButton_clicked() {

    if(player->state()==QMediaPlayer::PlayingState) {
        QIcon icon;
        icon.addFile(":/icon/mute");
        ui->musicButton->setIcon(icon);
        player->pause();
    }
    else {
        QIcon icon;
        icon.addFile(":/icon/sound");
        ui->musicButton->setIcon(icon);
        player->setVolume(25);
        player->play();
    }
}
