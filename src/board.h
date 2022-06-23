#ifndef BOARD_H
#define BOARD_H

#include "square.h"

class Board
{
public:
    Board();
    Board(int arr[81]);
    bool isBoardCompleted() const;
    void setBoardDigit(const int& col, const int& row, const int& digit, const bool& isPreset);
    int getBoardDigit(const int& col, const int& row);
    void performInitialBoardCheck();
    void performSquareGroupCheck(const int& squareGroupRow, const int& squareGroupCol);
    void checkForFinalDigits(Square& square);
    void checkForMarkups(Square& square);
    bool checkSafe(const Square& square, const int& digitToCheck) const;

private:
    Square _squares[9][9];
};

#endif // BOARD_H
