/*
#############################################################################
# COMP.CS.110 Programming 2, Autumn 2021                                    #
# Project 4: Minesweeper                                                    #
# File: mainwindow.cpp                                                      #
# Description: Describes the graphical aspect of the game                   #
#              Check the file mainwindow.hh for more information            #
# Program author                                                            #
# Name: Phuoc Nguyen                                                        #
# Student number: 150373014                                                 #
# UserID: qbphng                                                            #
# E-Mail: phuoc.nguyen@tuni.fi                                              #
#############################################################################
*/
#include "mainwindow.hh"
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    timer_(new QTimer(this)),
    hr(0), min(0), sec(0),
    graphicBoard_(new QWidget(this)),
    gridScaleFactor_(1),
    activeSquare_(nullptr)
{
    font_.setFamily(FONT_FAMILY);
    font_.setPointSize(FONT_SIZE);
    boldFont_ = font_;
    boldFont_.setBold(true);
    boldFont_.setPointSize(BOLD_FONT_SIZE);
    gridFont_ = boldFont_;
    gridFont_.setPointSize(GRID_FONT_SIZE);
    connect(timer_, SIGNAL(timeout()), this, SLOT(timeUpdate()));
    graphicBoard_->setLayout(createBoard());
    setCentralWidget(graphicBoard_);
    initialize();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initialize()
{
    setSeedGUI(true,
               "Enter seed (any integer), then click "
               "GO or press ENTER to begin the game",
               "Enter seed");
    setStatus("Waiting for seed..");
    setGridGUI(false);
    setDecisionGUI(false, "", S_EMPTY);
    for (auto &&b : squaresListGUI_) {
        b.first->blockSignals(false);
        b.first->setText("");
        b.first->setIcon(imgDir.at(I_NOTHING));
        b.second.state = S_EMPTY;
    }
}

QVBoxLayout* MainWindow::createBoard()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(graphicBoard_);

    mainLayout->addLayout(createSeedInputGUI());
    mainLayout->addLayout(createSquareGridGUI());
    mainLayout->addLayout(createStatusGUI());
    mainLayout->addLayout(createDecisionGUI());
//    graphicBoard_->setLayout(box);
    return mainLayout;
}

QGridLayout* MainWindow::createSeedInputGUI()
{
    QGridLayout* seedLayout = new QGridLayout();
    QLabel* title = new QLabel();
    QPixmap titleImg (imgDir.at(I_TITLE));
    titleImg = titleImg.scaled(TITLE_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    title->setPixmap(titleImg);
    seedLineEdit_ = new QLineEdit();
    seedLineEdit_->setPlaceholderText("");
    seedLineEdit_->setFixedSize(SEED_LINE_SIZE);
    seedLineEdit_->setAlignment(Qt::AlignCenter);
    connect(seedLineEdit_, SIGNAL(returnPressed()),
            this, SLOT(checkSeedValid()));
    getSeedButton = new QPushButton("GO");
    getSeedButton->setFixedSize(BUTTON_SIZE);
    connect(getSeedButton, SIGNAL(clicked()),
            this, SLOT(checkSeedValid()));
    gameInstructionText_ = new QLabel();
    gameInstructionText_->setAlignment(Qt::AlignCenter);
    gameInstructionText_->setFont(font_);
    restartButton_ = new QPushButton("RESTART");
    restartButton_->setFixedSize(BUTTON_SIZE);
    restartButton_->setVisible(false);
    closeButton_ = new QPushButton("EXIT");
    closeButton_->setFixedSize(BUTTON_SIZE);
    connect(restartButton_, SIGNAL(clicked()),
            this, SLOT(restartGame()));
    connect(closeButton_, SIGNAL(clicked()),
            this, SLOT(close()));
    QSlider* gridScaleSlider = new QSlider(Qt::Horizontal);
    gridScaleSlider->setMinimum(MIN_SCALE);
    gridScaleSlider->setMaximum(MAX_SCALE);
    gridScaleSlider->setFixedWidth(SLIDER_WIDTH);
    connect(gridScaleSlider, SIGNAL(valueChanged(int)),
            this, SLOT(changeScale(int)));
    seedLayout->addWidget(title, 0, 0, Qt::AlignHCenter);
    seedLayout->addItem(new QSpacerItem(100,100,QSizePolicy::Preferred,
                                         QSizePolicy::Preferred),0,0);
    seedLayout->addWidget(gameInstructionText_, 1, 0, 1, 0, Qt::AlignHCenter);
    seedLayout->addWidget(seedLineEdit_, 2, 0, Qt::AlignCenter);
    seedLayout->addWidget(getSeedButton, 3, 0, Qt::AlignHCenter);
    seedLayout->addWidget(restartButton_, 4, 0, Qt::AlignHCenter);
    seedLayout->addWidget(closeButton_, 5, 0, Qt::AlignHCenter);
    seedLayout->addWidget(gridScaleSlider, 6, 0, Qt::AlignHCenter);
    return seedLayout;
}

QGridLayout* MainWindow::createSquareGridGUI()
{
    QGridLayout* boardLayout = new QGridLayout();
    for (int row = 0; row < BOARD_SIDE; row++) {
        for (int column = 0; column < BOARD_SIDE; column++) {
            QLRClickButton* button = new QLRClickButton();
            button->setFixedSize(GRID_SIZE * gridScaleFactor_);
            button->setFont(gridFont_);
            connect(button, SIGNAL(leftClicked()),
                    this, SLOT(getDecision()));
            connect(button, SIGNAL(rightClicked()),
                    this, SLOT(quickFlag()));
            connect(button, SIGNAL(midClicked()),
                    this, SLOT(quickOpen()));
            ButtonProperties buttonProps;
            buttonProps.row = row;
            buttonProps.column = column;
            buttonProps.state = S_EMPTY;
            squaresListGUI_.insert(std::make_pair(button, buttonProps));

            boardLayout->addWidget(button, row+1, column+1);
        }
    }

    boardLayout->addItem(new QSpacerItem(1,1,QSizePolicy::Expanding,
                                         QSizePolicy::Preferred),1,0);
    boardLayout->addItem(new QSpacerItem(1,1,QSizePolicy::Expanding,
                                         QSizePolicy::Preferred),1, GRID_SIZE.height()+2);

    return boardLayout;
}

QGridLayout *MainWindow::createStatusGUI()
{
    QGridLayout* statusLayout = new QGridLayout();
    statusText_ = new QLabel();
    timerText_ = new QLabel();
    timerText_->setAlignment(Qt::AlignHCenter);
    timerText_->setFont(font_);
    statusText_->setFont(boldFont_);
    showTime();
    decisionButton_ = new QLRClickButton();
    decisionButton_->setFixedSize(DECISION_BUTTON_SIZE);
    decisionButton_->setIconSize(ICON_SIZE_LARGE);
    statusLayout->addWidget(statusText_, 0, 0, 1, 0, Qt::AlignHCenter);
    statusLayout->addWidget(timerText_, 1, 0, 1, 0, Qt::AlignHCenter);

    return statusLayout;
}

QGridLayout* MainWindow::createDecisionGUI()
{
    QGridLayout* decisionLayout = new QGridLayout();
    decisionInstructText_ = new QLabel();
    decisionInstructText_->setFont(font_);
    connect(decisionButton_, SIGNAL(leftClicked()), this, SLOT(cycleDecisions()));
    connect(decisionButton_, SIGNAL(rightClicked()), this, SLOT(confirmDecision()));

    decisionLayout->addWidget(decisionInstructText_, 0, 0, Qt::AlignHCenter);
    decisionLayout->addWidget(decisionButton_, 1, 0, Qt::AlignHCenter);
    decisionLayout->addItem(new QSpacerItem(1,1000,QSizePolicy::Preferred,
                                         QSizePolicy::Preferred),2,0);
    return decisionLayout;
}

QLRClickButton* MainWindow::getSquareFromCoord(const int row, const int column) const
{
    for (auto &&s : squaresListGUI_) {
        if (s.second.row == row && s.second.column == column) {
            return s.first;
        }
    }
    return nullptr;
}

SquareState MainWindow::getSquareState(QLRClickButton* square) const
{
    return squaresListGUI_.at(square).state;
}

void MainWindow::setSeedGUI(const bool visible, const QString text, const QString placeholder)
{
    gameInstructionText_->setText(text);
    seedLineEdit_->setText("");
    seedLineEdit_->setPlaceholderText(placeholder);
    seedLineEdit_->setVisible(visible);
    getSeedButton->setVisible(visible);
    restartButton_->setVisible(!visible);

}

void MainWindow::setGridGUI(bool enabled)
{
    for (auto &&b : squaresListGUI_) {
        b.first->setEnabled(enabled);
    }
}

void MainWindow::setDecisionGUI(bool visible, QString instructText, SquareState state)
{
    decisionInstructText_->setText(instructText);
    decisionButton_->setVisible(visible);
    tmpDecisionIdx = 0;
    availDecisions.clear();
    switch(state) {
    case S_EMPTY:
        availDecisions.push_back(I_PLACE_FLAG);
        availDecisions.push_back(I_CHECK);
        availDecisions.push_back(I_QUESTION_MARK);
        break;
    case S_FLAGGED:
        availDecisions.push_back(I_REMOVE_FLAG);
        availDecisions.push_back(I_QUESTION_MARK);
        availDecisions.push_back(I_KEEP_FLAG);
        break;
    case S_QUESTION_MARKED:
        availDecisions.push_back(I_PLACE_FLAG);
        availDecisions.push_back(I_CHECK);
        availDecisions.push_back(I_REMOVE_QUESTION_MARK);
        break;
    default:
        break;
    }
    if (visible) {
        decisionButton_->setIcon(imgDir.at(availDecisions[tmpDecisionIdx]));
    }
}

void MainWindow::showTime()
{
    QString timeShown = "Time spent\n"
                      + QString::number(hr) + ":"
                      + QString::number(min).rightJustified(2, '0') + ":"
                      + QString::number(sec).rightJustified(2, '0');
    timerText_->setText(timeShown);
}

void MainWindow::setStatus(QString msg)
{
    statusText_->setText(msg);
}

void MainWindow::changeScale(int scale)
{
    gridScaleFactor_ = float(scale)/100;
    QFont smallFontScaled = font_;
    QFont statusFontScaled = boldFont_;
    QFont gridGontScaled = gridFont_;
    smallFontScaled.setPointSize(FONT_SIZE*gridScaleFactor_);
    statusFontScaled.setPointSize(BOLD_FONT_SIZE*gridScaleFactor_);
    gridGontScaled.setPointSize(GRID_FONT_SIZE*gridScaleFactor_);
    decisionInstructText_->setFont(smallFontScaled);
    decisionButton_->setFixedSize(DECISION_BUTTON_SIZE*gridScaleFactor_);
    decisionButton_->setIconSize(ICON_SIZE_LARGE*gridScaleFactor_);
    for (auto &&b : squaresListGUI_) {
        b.first->setFixedSize(GRID_SIZE * gridScaleFactor_);
        b.first->setFont(gridGontScaled);
        b.first->setIconSize(ICON_SIZE_SMALL*gridScaleFactor_);

    }
    statusText_->setFont(statusFontScaled);
    timerText_->setFont(smallFontScaled);
}

void MainWindow::checkSeedValid()
{
    QString seed = seedLineEdit_->text();
    bool ok;
    int seedNum = seed.toInt(&ok, 10);

    if(!ok)
    {
        seedLineEdit_->setPlaceholderText("Seed needs to be an integer");
    }
    else
    {
        setGridGUI(true);
        emit requestStartGame(seedNum);

        timer_->start(1000);
        setStatus("SAFE ... FOR NOW");

        setSeedGUI(false,
                   "GAME INSTRUCTION:\nMines have been generated\n"
                   "Left click on a square and choose an action, \n"
                   "Right click on a square to quickly add or remove a flag\n"
                   "Middle click on a square to open and check the square\n"
                   "If you open a mine, the game is over! Flag all mines to win\n",
                   "");
    }
    seedLineEdit_->setText("");
}

void MainWindow::getDecision()
{
    activeSquare_ = qobject_cast<QLRClickButton*>(sender());

    setGridGUI(false);
    setDecisionGUI(true, "\n\n\n\nLeft click to toggle decisions, "
                         "right click to confirm decision",
                   getSquareState(activeSquare_));
}

void MainWindow::confirmDecision()
{
    switch(availDecisions[tmpDecisionIdx]) {
    case I_PLACE_FLAG:
        emit requestCmd(squaresListGUI_.at(activeSquare_).row,
                        squaresListGUI_.at(activeSquare_).column,
                        CMD_PLACE_FLAG);
        break;
    case I_REMOVE_FLAG:
        emit requestCmd(squaresListGUI_.at(activeSquare_).row,
                        squaresListGUI_.at(activeSquare_).column,
                        CMD_REMOVE_FLAG);
        break;
    case I_QUESTION_MARK:
        emit requestCmd(squaresListGUI_.at(activeSquare_).row,
                        squaresListGUI_.at(activeSquare_).column,
                        CMD_PLACE_QUESTION);
        break;
    case I_REMOVE_QUESTION_MARK:
        emit requestCmd(squaresListGUI_.at(activeSquare_).row,
                        squaresListGUI_.at(activeSquare_).column,
                        CMD_REMOVE_QUESTION);
        break;
    case I_CHECK:
        emit requestCmd(squaresListGUI_.at(activeSquare_).row,
                        squaresListGUI_.at(activeSquare_).column,
                        CMD_OPEN);
        break;
    default:
        break;
    }
    setGridGUI(true);
    setDecisionGUI(false, "", S_EMPTY);
    emit requestUpdate();
}

void MainWindow::quickFlag()
{
    activeSquare_ = qobject_cast<QLRClickButton*>(sender());
    SquareState activeState = getSquareState(activeSquare_);
    if (activeState == S_EMPTY) {
        emit requestCmd(squaresListGUI_.at(activeSquare_).row,
                        squaresListGUI_.at(activeSquare_).column,
                        CMD_PLACE_FLAG);
    }
    else if (activeState == S_FLAGGED) {
        emit requestCmd(squaresListGUI_.at(activeSquare_).row,
                        squaresListGUI_.at(activeSquare_).column,
                        CMD_REMOVE_FLAG);
    }
    emit requestUpdate();
}

void MainWindow::quickOpen()
{
    activeSquare_ = qobject_cast<QLRClickButton*>(sender());
    SquareState activeState = getSquareState(activeSquare_);
    if (activeState == S_EMPTY || activeState == S_QUESTION_MARKED) {
        emit requestCmd(squaresListGUI_.at(activeSquare_).row,
                        squaresListGUI_.at(activeSquare_).column,
                        CMD_OPEN);
    }
    emit requestUpdate();
}

void MainWindow::timeUpdate()
{
    sec++;
    if (sec == 60) {
        min++;
        sec = 0;
        if (min == 60) {
            min = 0;
            hr++;
        }
    }
    showTime();
}

void MainWindow::cycleDecisions()
{
    tmpDecisionIdx++;
    if (tmpDecisionIdx > int(availDecisions.size()-1)) {
        tmpDecisionIdx = 0;
    }
    decisionButton_->setIcon(imgDir.at(availDecisions[tmpDecisionIdx]));
}

void MainWindow::updateSquareIcon(int row, int col, SquareState state)
{
    QLRClickButton* button = getSquareFromCoord(row, col);
    switch(state) {
    case S_FLAGGED:
        button->setIcon(imgDir.at(I_HAS_FLAG));
        break;
    case S_QUESTION_MARKED:
        button->setIcon(imgDir.at(I_QUESTION_MARK));
        break;
    case S_EMPTY:
        button->setIcon(imgDir.at(I_NOTHING));
        break;
    case S_BOOM:
        button->setIcon(imgDir.at(I_BOOM));
        break;
    default:
        break;
    }
    button->setIconSize(ICON_SIZE_SMALL);
    squaresListGUI_.at(button).state = state;
}

void MainWindow::updateSquareAdjMine(int row, int col, int adjMines)
{
    QLRClickButton* button = getSquareFromCoord(row, col);
    button->setIcon(imgDir.at(I_NOTHING));
    button->setText(QString::number(adjMines));
    button->blockSignals(true);
    squaresListGUI_.at(button).state = S_OPENED;
}

void MainWindow::getBoom()
{
    setSeedGUI(false,
               "You opened a mine! Better luck next time!\n"
               "Press the RESTART button to replay the game",
               "");
    setStatus("BOOOOOOOOOOOOOOOM!");
    timer_->stop();
    setGridGUI(false);
}


void MainWindow::getGameOver()
{
    setSeedGUI(false,
               "Awesome! You flagged all the mines and won the game!\n"
               "Press the RESTART buton to replay the game",
               "");
    setStatus("YOU WON! CONGRATULATIONS!!!");
    timer_->stop();
    setGridGUI(false);
}

void MainWindow::restartGame()
{
    initialize();
    timer_->stop();
    hr = 0;
    min = 0;
    sec = 0;
    showTime();
    activeSquare_ = nullptr;
    emit requestRestart();
}

