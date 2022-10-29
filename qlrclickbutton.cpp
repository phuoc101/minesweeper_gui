/*
#############################################################################
# COMP.CS.110 Programming 2, Autumn 2021                                    #
# Project 4: Minesweeper                                                    #
# File: qlrclickbutton.cpp                                                  #
# Description: Implementation of a push button that accepts left, right and #
#              middle clicks                                                #
#              Check the file qlrclickbutton.hh for more information        #
# Program author                                                            #
# Name: Phuoc Nguyen                                                        #
# Student number: 150373014                                                 #
# UserID: qbphng                                                            #
# E-Mail: phuoc.nguyen@tuni.fi                                              #
#############################################################################
*/
#include "qlrclickbutton.hh"

QLRClickButton::QLRClickButton(QWidget *parent) :
    QPushButton(parent)
{
}

void QLRClickButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::RightButton) {
        emit rightClicked();
    }
    else if (e->button()==Qt::LeftButton) {
        emit leftClicked();
    }
    if(e->button()==Qt::MiddleButton) {
        emit midClicked();
    }
}
