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
    this->wait();
}

void Board::setBoardMarkup(const int &row, const int &col, const int &digit, const bool &remove)
{
    if (remove)
    {
        this->_squares[row][col].removeMarkup(digit);
        emit this->uiRemoveMarkup(row, col, digit);
    }
    else
    {
        if (this->_squares[row][col].setMarkup(digit))
        {
            emit this->uiAddMarkup(row, col, digit);
        }
    }
    this->wait();
}

int Board::getBoardDigit(const int &row, const int &col)
{
    return this->_squares[row][col].getDigit();
}

void Board::performInitialBoardCheck()
{
    this->useAdvancedRules = false;
    makeCheck(0, 0);

    if (!this->isBoardCompleted())
    {
        this->useAdvancedRules = true;
        makeCheck(0, 0);
    }
    if (!this->isBoardCompleted())
    {
        this->strategiesUsed.insert("Backtracking");
        Backtracking::doBacktracking(*this);
    }
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
    QString solvingSummary = "", strategiesSummary = "";

    this->strategiesUsed = std::set<std::string>();
    std::cout << "--Solving Started--" << std::endl;

    auto t1 = std::chrono::high_resolution_clock::now();
    this->performInitialBoardCheck();
    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms_double = t2 - t1;

    if (this->isBoardCompleted())
    {
        solvingSummary = "Solving Completed, Took " + QString::number(ms_double.count() / 1000) + "s";
        strategiesSummary = "Solved using: " + QString::fromStdString(Helper::set_to_string(this->strategiesUsed));
        emit this->uiWriteToLog(solvingSummary, false);
        emit this->uiWriteToStrategiesLabel(strategiesSummary);
    }
    else
    {
        solvingSummary = "Invalid sudoku, has more than one solution";
        emit this->uiWriteToLog(solvingSummary, true);
    }
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
                    this->strategiesUsed.insert("Simple check");
                    this->setBoardDigit(i, j, finalDigit, false);
                    recheckGroups = 1;
                }
            }
        }
    }

    if (this->useAdvancedRules) {
        if (HiddenSingles::checkForHiddenSingles(*this)) {
            this->strategiesUsed.insert("Hidden Singles");
            recheckGroups = 1;
        }
        if (UniqueRectangle::checkForUniqueRectangle(*this))
        {
            recheckGroups = 1;
        }
        if (NakedTriples::checkForNakedTriples(*this))
        {
            this->strategiesUsed.insert("Naked Triples");
            recheckGroups = 1;
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
        if (square.getDigit() == 0 && checkSafe(square, i))
        {
            this->setBoardMarkup(square.getRow(), square.getCol(), i, false);
        }
        else
        {
            if (square.digitMarkupExists(i))
            {
                this->setBoardMarkup(square.getRow(), square.getCol(), i, true);
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
