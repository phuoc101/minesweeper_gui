/*
#############################################################################
# COMP.CS.110 Programming 2, Autumn 2021                                    #
# Project 4: Minesweeper                                                    #
# File: gameengine.hh                                                       #
# Description:                                                              #
#     _This file contains declaration for the functions of the game engine. #
#     _The game is played as followed:                                      #
#        _First, user enters a seed to generate bombs                       #
#        _Then the user can either choose between one of the decisions:     #
#            _Place a flag or remove a flag. A square with a flag cannot    #
#            be opened.                                                     #
#            _Place a question mark or remove a question mark. A question   #
#            mark square can be changed to a flag, or opened. Placing a     #
#            question mark also removes the flag, and vice versa.           #
#            _Check the square to see if there's a bomb. If there's a bomb, #
#            the player loses. If there's not a bomb, it shows the number   #
#            of adjacent bomb and if the adjacent squares also doesn't have #
#            a bomb, it opens it also.                                      #
#        _The game is won when the player successfully flags all the bombs. #
# Program author                                                            #
# Name: Phuoc Nguyen                                                        #
# Student number: 150373014                                                 #
# UserID: qbphng                                                            #
# E-Mail: phuoc.nguyen@tuni.fi                                              #
#############################################################################
*/

#ifndef GAMEENGINE_HH
#define GAMEENGINE_HH

#include "square.hh"
#include "gameboard.hh"
#include <QObject>
#include <random>
#include <map>

// list of all possible states for square
enum SquareState {S_EMPTY, S_OPENED, S_FLAGGED, S_QUESTION_MARKED, S_BOOM};
enum CmdCode {CMD_PLACE_FLAG, CMD_REMOVE_FLAG,
              CMD_PLACE_QUESTION, CMD_REMOVE_QUESTION,
              CMD_OPEN};

class GameEngine : public QObject
{
    Q_OBJECT
public:
    GameEngine();

    // add a flag at coordinate given
    void flagSquare(int row, int col);

    // remove a flag at coordinate given
    void rmFlagSquare(int row, int col);

    // add a question mark at coordinate given
    void questionSquare(int row, int col);

    // remove a question mark at coordinate given
    void rmQuestionSquare(int row, int col);

    // open a square at the given coordinate
    void openSquare(int row, int col);

signals:
    // signal to send update about the square icon
    void sendUpdateSquareIcon(int row, int col, SquareState state);

    // signal to send update about the square adjacent mine
    void sendUpdateSquareAdjMine(int row, int col, int adjMine);

    // signal to send update about win condition
    void sendGameOver();

    // signal to send update about lose condition
    void sendBoom();

    //signal to end game
    void quitGame();

public slots:
    // slot to start game with given seed
    void startGame(int seed);

    // slot to restart the game
    void restartGame();

    // slot to handle command given at row and column
    void handleCmd(int row, int col, CmdCode code);

    // slot to update the board
    void updateBoard();

private:
    GameBoard board_;
};

#endif // GAMEENGINE_HH
