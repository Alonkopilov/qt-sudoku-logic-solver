#ifndef BOARD_H
#define BOARD_H

#include "square.h"
#include <QObject>
#include <iostream>

class Board : public QObject
{
    Q_OBJECT

public:
    Board(int arr[81]);
    bool isBoardCompleted() const;
    void performInitialBoardCheck();

public slots:
    int checkForFinalDigit(Square& square);
    void checkForMarkups(Square& square);
    void setBoardDigit(const int& col, const int& row, const int& digit, const bool& isPreset);

private:
    int performSquareGroupCheck(const int& squareGroupRow, const int& squareGroupCol);
    bool checkSafe(const Square& square, const int& digitToCheck) const;
    int getBoardDigit(const int& col, const int& row);
    Square _squares[9][9];
};

#endif // BOARD_H
