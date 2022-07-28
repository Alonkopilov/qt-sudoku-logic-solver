#include "board.h"

Board::Board()
{
}

bool Board::isBoardCompleted() const
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (this->_squares[i][j].getDigit() == 0)
            {
                return false;
            }
        }
    }

    return true;
}

void Board::setBoardDigit(const int &row, const int &col, const int &digit, const bool &isPreset)
{
    this->_squares[row][col].setDigit(digit, isPreset); // Set in memory
    emit this->uiSetBoardDigit(row, col, digit, isPreset); // Set in GUI
}

int Board::getBoardDigit(const int &row, const int &col)
{
    return this->_squares[row][col].getDigit();
}

void Board::performInitialBoardCheck()
{
    makeCheck(0, 0);
}

void Board::makeCheck(int j, int i)
{
    if (j == 3 && i == 3 && performSquareGroupCheck(i, j) == 0) {
        return;
    }
    if (performSquareGroupCheck(i, j) == 1)
    {
        makeCheck(0, 0);
    }
    else
    {
        if (j < 2)
        {
            makeCheck(j + 1, i);
        }
        if (i < 2)
        {
            makeCheck(j, i + 1);
        }
    }

}

void Board::wait()
{
    if (this->isSlowSolver)
    {
        QThread::msleep(10);
    }
}

void Board::initializeBoard(const int arr[81])
{
    for (int i = 0; i < 81; i++)
    {
        int row = i / 9;
        int col = i % 9;
        int digit = arr[i];
        this->_squares[row][col] = Square(row, col);
        this->setBoardDigit(row, col, digit, true);
    }
}

void Board::toggleSlowSolve(const bool &isSlow)
{
    this->isSlowSolver = isSlow;
}

void Board::run() {
    std::cout << "--Solving Started--" << std::endl;
    this->performInitialBoardCheck();
}

int Board::performSquareGroupCheck(const int &squareGroupRow, const int &squareGroupCol)
{
    int recheckGroups = 0;

    // Check all group squares for markups, and final digits
    for (int i = squareGroupRow * 3; i < squareGroupRow * 3 + 3; i++)
    {
        for (int j = squareGroupCol * 3; j < squareGroupCol * 3 + 3; j++)
        {
            if (getBoardDigit(i, j) == 0)
            {
                checkForMarkups(_squares[i][j]);
                int finalDigit = checkForFinalDigit(_squares[i][j]);

                if (finalDigit > 0)
                {
                    this->setBoardDigit(i, j, finalDigit, false);
                    recheckGroups = 1;
                }
            }
        }
    }

    // Check for markups that only appear once in the square group

    int arr[9] = { 0 };
    int digit = -1;
    for (int i = squareGroupRow * 3; i < squareGroupRow * 3 + 3; i++)
    {
        for (int j = squareGroupCol * 3; j < squareGroupCol * 3 + 3; j++)
        {
            if (getBoardDigit(i, j) == 0)
            {
                for (int m = 0; m < 9; m++)
                {
                    if (this->_squares[i][j].digitMarkupExists(m + 1))
                    {
                       arr[m]++;
                    }
                }
            }
        }
    }

    for (int i = 0; i < 9 && digit == -1; i++)
    {
        if (arr[i] == 1)
        {
            digit = i + 1;
        }
    }
    if (digit == -1)
    {
        return recheckGroups;
    }

    for (int i = squareGroupRow * 3; i < squareGroupRow * 3 + 3; i++)
    {
        for (int j = squareGroupCol * 3; j < squareGroupCol * 3 + 3; j++)
        {
            if (getBoardDigit(i, j) == 0 && checkSafe(this->_squares[i][j], digit) && this->_squares[i][j].digitMarkupExists(digit))
            {
                setBoardDigit(i, j, digit, false);
                recheckGroups = 1;
            }
        }
    }

    return recheckGroups;
}

int Board::checkForFinalDigit(Square &square)
{
    int markupsFound = 0;
    int markup = 0;

    for (int i = 1; i < 10; i++)
    {
        if (square.digitMarkupExists(i)) {
            markupsFound++;
            markup = i;
        }
        if (markupsFound > 1){
            return 0;
        }
    }
    return markup;
}

void Board::checkForMarkups(Square &square)
{
    for (int i = 1; i < 10; i++)
    {
        if (checkSafe(square, i))
        {
            square.setMarkup(i);
            emit this->uiAddMarkup(square.getRow(), square.getCol(), i);
            this->wait();
        }
        else
        {
            if (square.digitMarkupExists(i))
            {
                square.removeMarkup(i);
                emit this->uiRemoveMarkup(square.getRow(), square.getCol(), i);
                this->wait();
            }
        }
    }
}

bool Board::checkSafe(const Square &square, const int &digitToCheck) const
{
    for (int i = 0; i < 9; i++) //Check row
    {
        if (_squares[square.getRow()][i].getDigit() == digitToCheck)
        {
            return false;
        }
    }

    for (int i = 0; i < 9; i++) //Check column
    {
        if (_squares[i][square.getCol()].getDigit() == digitToCheck)
        {
            return false;
        }
    }

    int groupRow = square.getRow() / 3;
    int groupCol = square.getCol() / 3;

    for (int i = groupRow * 3; i < groupRow * 3 + 3; i++) //Check square group
    {
        for (int j = groupCol * 3; j < groupCol * 3 + 3; j++)
        {
            if (_squares[i][j].getDigit() == digitToCheck)
            {
                return false;
            }
        }
    }

    return true;
}
