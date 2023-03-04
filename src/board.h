#ifndef BOARD_H
#define BOARD_H

#include "square.h"
#include "logic/nakedtriples.h"
#include "logic/backtracking.h"
#include "logic/uniquerectangle.h"
#include "logic/nakedpairs.h"
#include "logic/hiddensingles.h"
#include "utils/helper.h"
#include <QObject>
#include <iostream>
#include <QThread>
#include <chrono>

enum NakedPairPlacement {
    inSameRow,
    inSameColumn,
    inSameSquareGroup
};

#define N 9

/// @brief Represents the Sudoku board.
/// This class contains functions to enter/remove digits and markups, solve the board, and initialize it.
class Board : public QThread
{
    Q_OBJECT

public:
    Board();

    /// @brief Checks if the board was solved successfully.
    bool isBoardCompleted() const;

    /// @brief Initializes the board with a given problem.
    /// @param arr The Sudoku board as a one dimensional array
    void initializeBoard(const int arr[81]);

    /// @brief Turns on/off slow solving. Used mainly to demonstrate the solving process in the GUI.
    /// @param isSlow True if slow solving should be turned on, else false.
    void toggleSlowSolve(const bool& isSlow);

    void run() override; // Starts the solver

signals:
    void uiSetBoardDigit(const int &row, const int &col, const int &digit, const bool &isPreset);
    void uiAddMarkup(const int& row, const int& col, const int& digit);
    void uiRemoveMarkup(const int& row, const int& col, const int& digit);
    void uiWriteToLog(const QString& str, const bool& isErr);
    void uiWriteToStrategiesLabel(const QString& str);

private:
    /// @brief Starts the solving process. From simple checks, to more advanced strategies when needed.
    void solve();

    /// @brief Checks if a final digit can be put on a square.
    /// A square is considered final if it has only one markup = only one digit can be put in it for the solution. 
    /// @param square The square to check
    /// @return If the square is final, returns the final digit for the solution, else 0.
    int checkForFinalDigit(Square& square);

    /// @brief Checks if digit markups can be put in a given square and sets them up on the board.
    /// @param square The square to check for new markups on.
    void checkForMarkups(Square& square);

    /// @brief Sets a final digit on the board - in memory + in GUI.
    /// @param row The row index
    /// @param col The col index
    /// @param digit The digit to write
    /// @param isPreset True if the digit is a part of the Sudoku problem, false if the digit is a part of the solution.
    void setBoardDigit(const int &row, const int &col, const int &digit, const bool &isPreset);

    /// @brief Adds/removes a digit markup on/from the board.
    /// @param row The row index
    /// @param col The col index
    /// @param digit The digit of the markup
    /// @param remove True if the markup should be removed, false if it should be added
    void setBoardMarkup(const int &row, const int &col, const int &digit, const bool &remove);

    /// @brief Starts the Sudoku solving strategies on the given square group indexes.\n
    /// A square group is equal to a 3x3 group of squares on the board. There are a total of 9 groups
    /// @param squareGroupRow The row index
    /// @param squareGroupCol The col index
    /// @return 1 if one of the solving stategies resulted in one or more final digits in the square group, else 0.
    int performSquareGroupCheck(const int& squareGroupRow, const int& squareGroupCol);

    /// @brief Checks if it is safe for given digit to be put in the given square.\n 
    /// A square is considered "safe" when the given digit does not appear again in the same square group, row, or column.
    /// **Important - A digit that is considered "safe" to be put in a square, does NOT mean it will be there in the solution,
    /// it may be for a markup only.
    /// @param square The square to test the digit in
    /// @param digitToCheck The digit to check
    /// @return True if it is possible for the digit to be in the square, else false.
    bool checkSafe(const Square& square, const int& digitToCheck) const;

    int getBoardDigit(const int &row, const int &col);

    /// @brief Recursively tries solving strategies on square groups until the board is completely solved.
    /// @param i The square group row index to start with.
    /// @param j The square group column index to start with.
    void makeCheck(int i, int j);

    /// @brief Sleeps the solving thread for a small amount of time if "slow solving" is enabled (this.isSlowSolver == true).
    /// Slow solving is a feature used to demonstrate the solving process in the GUI.
    void wait();

    Square _squares[N][N];
    bool isSlowSolver;
    bool useAdvancedRules; // Signals the solver to start using advanced solving strategies.
    std::set<std::string> strategiesUsed; // Keeps track of what solving strategies were used to show in the GUI.

friend class HiddenSingles;
friend class NakedPairs;
friend class NakedTriples;
friend class Backtracking;
friend class UniqueRectangle;
};

#endif // BOARD_H
