#include "mainwindow.hh"
#include <QApplication>
#include <iostream>

int main(int argc, char* argv[])
{
    GameEngine engine;
    QApplication a(argc, argv);
    MainWindow w;
    QObject::connect(&w, SIGNAL(requestStartGame(int)),
                     &engine, SLOT(startGame(int)));
    QObject::connect(&w, SIGNAL(requestRestart()),
                     &engine, SLOT(restartGame()));
    QObject::connect(&engine, SIGNAL(sendBoom()),
                     &w, SLOT(getBoom()));
    QObject::connect(&engine, SIGNAL(sendGameOver()),
                     &w, SLOT(getGameOver()));

    QObject::connect(&w, SIGNAL(requestCmd(int,int,CmdCode)),
                     &engine, SLOT(handleCmd(int,int,CmdCode)));
    QObject::connect(&w, SIGNAL(requestUpdate()),
                     &engine, SLOT(updateBoard()));

    QObject::connect(&engine, SIGNAL(sendUpdateSquareIcon(int,int,SquareState)),
                     &w, SLOT(updateSquareIcon(int,int,SquareState)));
    QObject::connect(&engine, SIGNAL(sendUpdateSquareAdjMine(int,int,int)),
                     &w, SLOT(updateSquareAdjMine(int,int,int)));
    w.show();
    return a.exec();
}
