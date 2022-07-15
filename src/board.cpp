#include "board.h"

Board::Board(int arr[81])
{
    for (int i = 0; i < 81; i++)
    {
        int row = i / 9;
        int col = i % 9;
        int digit = arr[i];

        this->_squares[row][col] = Square(digit, true, row, col);
        this->setBoardDigit(col, row, digit, true);
    }
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

void Board::setBoardDigit(const int &col, const int &row, const int &digit, const bool &isPreset)
{
    this->_squares[col][row].setDigit(digit, isPreset);
}

int Board::getBoardDigit(const int &col, const int &row)
{
    return this->_squares[col][row].getDigit();
}

void Board::performInitialBoardCheck()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            performSquareGroupCheck(i, j);
        }
    }
}

void Board::performSquareGroupCheck(const int &squareGroupRow, const int &squareGroupCol)
{
    for (int i = squareGroupRow * 3; i < squareGroupRow * 3 + 2; i++)
    {
        for (int j = squareGroupCol * 3; j < squareGroupCol * 3 + 2; j++)
        {
            if (getBoardDigit(i, j) == 0)
            {
                checkForMarkups(_squares[i][j]);
                int finalDigit = checkForFinalDigit(_squares[i][j]);

                if (finalDigit > 0) {

                }
            }
        }
    }
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
        checkSafe(square, i) ? square.setMarkup(i) : square.removeMarkup(i);
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

    for (int i = groupRow * 3; i < 3; i++) //Check square group
    {
        for (int j = groupCol * 3; j < 3; j++)
        {
            if (_squares[i][j].getDigit() == digitToCheck)
            {
                return false;
            }
        }
    }

    return true;
}
