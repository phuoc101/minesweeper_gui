/*
#############################################################################
# COMP.CS.110 Programming 2, Autumn 2021                                    #
# Project 4: Minesweeper                                                    #
# File: qlrclickbutton.hh                                                       #
# Description:                                                              #
#     _This file contains declaration for the functions of the class        #
#     QLRClickButton, which inherits from the class QPushButton, the        #
#     function mousePressEvent was overridden to accepts left, right and    #
#     middle click.                                                         #
# Program author                                                            #
# Name: Phuoc Nguyen                                                        #
# Student number: 150373014                                                 #
# UserID: qbphng                                                            #
# E-Mail: phuoc.nguyen@tuni.fi                                              #
#############################################################################
*/
#ifndef QLRCLICKBUTTON_HH
#define QLRCLICKBUTTON_HH

#include <QPushButton>
#include <QMouseEvent>
#include <QObject>

class QLRClickButton : public QPushButton
{
    Q_OBJECT
public:
    explicit QLRClickButton(QWidget *parent = nullptr);

signals:
    void leftClicked();
    void rightClicked();
    void midClicked();
private slots:
    void mousePressEvent(QMouseEvent *e) override;

};

#endif // QLRCLICKBUTTON_HH
