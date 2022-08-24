#ifndef BOARD_H
#define BOARD_H

#include "square.h"
#include <QObject>
#include <iostream>
#include <QThread>
#include <chrono>

class Board : public QThread
{
    Q_OBJECT

public:
    Board();
    bool isBoardCompleted() const;
    void initializeBoard(const int arr[81]);
    void toggleSlowSolve(const bool& isSlow);
    void run() override; // Start solver

signals:
    void uiSetBoardDigit(const int &row, const int &col, const int &digit, const bool &isPreset);
    void uiAddMarkup(const int& row, const int& col, const int& digit);
    void uiRemoveMarkup(const int& row, const int& col, const int& digit);
    void uiWriteToLog(const QString& str, const bool& isErr);

private:
    void performInitialBoardCheck();

    bool checkForUniqueRectangle();
    bool checkForOneMarkupAppearanceOfDigit(const int &squareGroupRow, const int &squareGroupCol);

    int checkForFinalDigit(Square& square);
    void checkForMarkups(Square& square);
    void setBoardDigit(const int &row, const int &col, const int &digit, const bool &isPreset);
    int performSquareGroupCheck(const int& squareGroupRow, const int& squareGroupCol);
    bool checkSafe(const Square& square, const int& digitToCheck) const;
    int getBoardDigit(const int &row, const int &col);
    void makeCheck(int i, int j);
    void wait();

    Square _squares[9][9];
    bool isSlowSolver;
    bool useAdvancedRules;
};

#endif // BOARD_H
