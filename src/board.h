#ifndef BOARD_H
#define BOARD_H

#include "square.h"
#include <QObject>
#include <iostream>
#include <algorithm>

class Board : public QObject
{
    Q_OBJECT

public:
    Board();
    bool isBoardCompleted() const;
    void performInitialBoardCheck();
    void initializeBoard(int arr[81]);

signals:
    void uiSetBoardDigit(const int &row, const int &col, const int &digit, const bool &isPreset);
    void uiAddMarkup(const int& row, const int& col, const int& digit);
    void uiRemoveMarkup(const int& row, const int& col, const int& digit);

private:
    int checkForFinalDigit(Square& square);
    void checkForMarkups(Square& square);
    void setBoardDigit(const int &row, const int &col, const int &digit, const bool &isPreset);
    int performSquareGroupCheck(const int& squareGroupRow, const int& squareGroupCol);
    bool checkSafe(const Square& square, const int& digitToCheck) const;
    int getBoardDigit(const int &row, const int &col);
    void makeCheck(int i, int j);
    Square _squares[9][9];
};

#endif // BOARD_H
