/*
#############################################################################
# COMP.CS.110 Programming 2, Autumn 2021                                    #
# Project 4: Minesweeper                                                    #
# File: gameengine.cpp                                                      #
# Description: Describes the logical aspect of the game                     #
#              Check the file gameengine.hh for more information            #
# Program author                                                            #
# Name: Phuoc Nguyen                                                        #
# Student number: 150373014                                                 #
# UserID: qbphng                                                            #
# E-Mail: phuoc.nguyen@tuni.fi                                              #
#############################################################################
*/
#include "gameengine.hh"
#include <QMessageBox>


GameEngine::GameEngine():
    QObject(0)
{
}

void GameEngine::startGame(int seed)
{
    board_.init(seed);
}

void GameEngine::restartGame()
{
    board_.reset();
}

void GameEngine::updateBoard()
{
    bool boomed = false;
    for (int y = 0; y < BOARD_SIDE; y++) {
        for (int x = 0; x < BOARD_SIDE; x++) {
            Square sq = board_.getSquare(x, y);
            if (sq.hasFlag()) {
                emit sendUpdateSquareIcon(y, x, S_FLAGGED);
            }
            else if (sq.hasQuestion()) {
                emit sendUpdateSquareIcon(y, x, S_QUESTION_MARKED);
            }
            else if (!sq.isOpen()) {
                emit sendUpdateSquareIcon(y, x, S_EMPTY);
            }
            else if (sq.isOpen()) {
                if (!sq.hasMine()) {
                    emit sendUpdateSquareAdjMine(y, x, sq.countAdjacent());
                }
                else {
                    boomed = true;
                    emit sendUpdateSquareIcon(y, x, S_BOOM);
                    emit sendBoom();
                    break;
                }
            }
        }
        if (boomed) {
            break;
        }
    }
    if (board_.isGameOver()) {
        emit sendGameOver();
    }
}

void GameEngine::flagSquare(int row, int col)
{
    Square sq = board_.getSquare(col, row);
    sq.addFlag();
    board_.setSquare(sq, col, row);
}

void GameEngine::rmFlagSquare(int row, int col)
{
    Square sq = board_.getSquare(col, row);
    sq.removeFlag();
    board_.setSquare(sq, col, row);
}

void GameEngine::questionSquare(int row, int col)
{
    Square sq = board_.getSquare(col, row);
    sq.addQuestionMark();
    board_.setSquare(sq, col, row);
}

void GameEngine::rmQuestionSquare(int row, int col)
{
    Square sq = board_.getSquare(col, row);
    sq.removeQuestionMark();
    board_.setSquare(sq, col, row);
}

void GameEngine::openSquare(int row, int col)
{
    board_.openSquare(col, row);
}



void GameEngine::handleCmd(int row, int col, CmdCode code)
{
    switch (code) {
    case CMD_PLACE_FLAG:
        flagSquare(row, col);
        break;
    case CMD_REMOVE_FLAG:
        rmFlagSquare(row, col);
        break;
    case CMD_PLACE_QUESTION:
        questionSquare(row, col);
        break;
    case CMD_REMOVE_QUESTION:
        rmQuestionSquare(row, col);
        break;
    case CMD_OPEN:
        openSquare(row, col);
        break;
    }
}
