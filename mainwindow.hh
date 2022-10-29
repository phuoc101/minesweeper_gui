/*
#############################################################################
# COMP.CS.110 Programming 2, Autumn 2021                                    #
# Project 4: Minesweeper                                                    #
# File: mainwindow.hh                                                       #
# Description:                                                              #
#    _This file contains declarations for the components and functions      #
#    implemented for the GUI of the game.                                   #
#     _The GUI has 4 main layouts: seed selection, the game grid, the status#
#     area, and the decision button.                                        #
#     _The seed selection area first takes input from the user and pass it  #
#     to the game engine to start the game.                                 #
#     _The game grid is linked to the engine to run the game. They are      #
#     implemented as push buttons with icons.                               #
#     _The status area shows the whether the player is safe, has opened a   #
#     bomb, or won the game. Also, there is a timer showing the time spent  #
#     on the round.                                                         #
#     _The extra features implemented for this project:                     #
#        _There is an option for setting a question mark                    #
#        _Instead of printing a character in an opening square, a suitable  #
#        figure or icon will be shown                                       #
#        _Instead of using radio button to choose decisions, a single push  #
#        button is used for this. It cycles through all the available       #
#        choices with a left click, and a decision is made with a right     #
#        click.                                                             #
#        _There is another option to quickly add or remove a flag with      #
#        a right click. Also, player can quickly open a square with a middle#
#        click.                                                             #
#        _There is a slider for user to change the scale of the grid, the   #
#        text and image inside the buttons, the status and timer text.      #
# Program author                                                            #
# Name: Phuoc Nguyen                                                        #
# Student number: 150373014                                                 #
# UserID: qbphng                                                            #
# E-Mail: phuoc.nguyen@tuni.fi                                              #
#############################################################################
*/
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "gameboard.hh"
#include "gameengine.hh"
#include "qlrclickbutton.hh"
#include <QPushButton>
#include <QMainWindow>
#include <QGraphicsView>
#include <QBoxLayout>
#include <QLineEdit>
#include <QSlider>
#include <QLabel>
#include <QTimer>
#include <QFont>
#include <QDir>
#include <map>

// List of all the image codes (images are in the Resources folder)
enum ImgCode {I_TITLE, I_NOTHING, I_HAS_FLAG, I_PLACE_FLAG, I_REMOVE_FLAG, I_CHECK,
              I_QUESTION_MARK, I_REMOVE_QUESTION_MARK, I_KEEP_FLAG, I_BOOM};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    // signal to request the engine to start the game with given seed
    void requestStartGame(int seed);

    // signal to request the engine to handle a command code for the square
    // at the given row and column
    void requestCmd(int row, int col, CmdCode code);

    // signal to request the engine to scan the board and update every square
    void requestUpdate();

    // signal to request the game to restart
    void requestRestart();

private slots:
    // Slot to change the scale of the board grids the decision button and
    // other text components
    void changeScale(int scale);

    // Slot to check whether the seed is valid
    void checkSeedValid();

    // Slot to show the decision button to get the decision
    void getDecision();

    // Slot to cycle through the possible decisions
    void cycleDecisions();

    // Slot to confirm the decision of the decision button
    void confirmDecision();

    // Slot to handle quick flag by right click
    void quickFlag();

    // Slot to handle quick open by middle click
    void quickOpen();

    // Slot to update time
    void timeUpdate();

    // Slot to update a Square icon depending on the row, column and state
    // sent by signal from engine
    void updateSquareIcon(int row, int col, SquareState state);

    // Slot to update a Square if it shows adjacent mines depending on the
    // row, column and number of adjacent mines sent by signal from engine
    void updateSquareAdjMine(int row, int col, int adjMines);

    // Slot to handle lose condition (openning a mine)
    void getBoom();

    // Slot to handle win condition (flagging all mines)
    void getGameOver();

    // Slot to reset GUI elements and prepare for another round
    void restartGame();

private:
    // Initialize the components of the GUI program.
    void initialize();

    // Create the board by combining the 4 layout components: the seed input,
    // the square grid, the status area and the decision button
    QVBoxLayout* createBoard();

    // Create the GUI to get user seed input, after that it shows the game
    // instruction
    QGridLayout* createSeedInputGUI();

    // Create the board grid with imaged push buttons
    QGridLayout* createSquareGridGUI();

    // Create the area for the status of the game
    QGridLayout* createStatusGUI();

    // Create the Decision area layout
    QGridLayout* createDecisionGUI();

    // Get the pointer to the square from row, column
    QLRClickButton* getSquareFromCoord(const int row, const int col) const;

    // get the state of the square
    SquareState getSquareState(QLRClickButton* square) const;

    // configure the seed input GUI layout
    // Parameters:
    // _visible: set whether the line edit to get the seed is visible or not
    // _text: the instructional text for users
    // _placeholder: the placeholder text of the line edit to help users
    void setSeedGUI(const bool visible, const QString text,
                    const QString placeholder);

    // configure the grid
    // Parameters:
    // _enabled: enabling the push buttons or not
    void setGridGUI(const bool enabled);

    // configure the decision GUI layout:
    // Parameters:
    // _visible: set whether the Decision button is visible or not
    // _instructText: instructional text to guide users
    // _code: the state of the square sending the request for decision,
    // different states have different decisions
    void setDecisionGUI(const bool visible, const QString instructText,
                        const SquareState code);

    // show the timer time;
    void showTime();

    // set the status text message
    void setStatus(QString msg);
    // data structure containing properties of each square
    struct ButtonProperties
    {
        int row;
        int column;
        SquareState state;
    };

    // list of resource images, called by the image code
    const std::map<ImgCode, QPixmap> imgDir = {
        {I_TITLE, QPixmap("../minesweeper_gui/Resources/title.png")},
        {I_PLACE_FLAG, QPixmap("../minesweeper_gui/Resources/place_flag.png")},
        {I_NOTHING, QPixmap("")},
        {I_CHECK, QPixmap("../minesweeper_gui/Resources/check.png")},
        {I_REMOVE_FLAG, QPixmap("../minesweeper_gui/Resources/rm_flag.png")},
        {I_HAS_FLAG, QPixmap("../minesweeper_gui/Resources/flag.png")},
        {I_QUESTION_MARK, QPixmap("../minesweeper_gui/Resources/question.png")},
        {I_REMOVE_QUESTION_MARK, QPixmap("../minesweeper_gui/Resources/rm_question.png")},
        {I_KEEP_FLAG, QPixmap("../minesweeper_gui/Resources/keep_flag.png")},
        {I_BOOM, QPixmap("../minesweeper_gui/Resources/boom.png")}
    };

    // constant sizes for GUI elements
    const QSize TITLE_SIZE = QSize(600,200);
    const QSize GRID_SIZE = QSize(40, 40);
    const QSize ICON_SIZE_SMALL = QSize(30, 30);
    const QSize BUTTON_SIZE = QSize(90, 20);
    const QSize SEED_LINE_SIZE = QSize(200, 20);
    const QSize DECISION_BUTTON_SIZE = QSize(80, 80);
    const QSize ICON_SIZE_LARGE = QSize(70, 70);
    const QString FONT_FAMILY = "Helvetica";
    const int MIN_SCALE = 100;
    const int MAX_SCALE = 130;
    const int SLIDER_WIDTH = 300;
    const int FONT_SIZE = 12;
    const int BOLD_FONT_SIZE = 15;
    const int GRID_FONT_SIZE = 16;

    // GUI components for timer
    QTimer* timer_;
    int hr;
    int min;
    int sec;
    QLabel* timerText_;

    QLabel* statusText_;

    QPushButton* restartButton_;
    QPushButton* closeButton_;

    //GUI components for seed input
    QLineEdit* seedLineEdit_;
    QWidget* graphicBoard_;
    QPushButton* getSeedButton;
    QLabel* gameInstructionText_;

    // scale factor of grid
    float gridScaleFactor_;

    // components to support decision button
    int tmpDecisionIdx;
    QLabel* decisionInstructText_;
    QLRClickButton* decisionButton_;
    std::vector<ImgCode> availDecisions;

    // the square that was just clicked
    QLRClickButton* activeSquare_;
    std::map<QLRClickButton*, ButtonProperties> squaresListGUI_;

    // font GUI components
    QFont boldFont_;
    QFont font_;
    QFont gridFont_;
};
#endif // MAINWINDOW_HH
