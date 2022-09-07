#include "nakedtriples.h"

bool NakedTriples::checkForNakedTriples(Board &board)
{
    int recheckGroups = 0;

    for (int i = 0; i < 9; i++)
    {
        // Start algorithm on each row and column
        if (NakedTriples::checkNakedTripleInUnit(board, i, Units::ROW) || NakedTriples::checkNakedTripleInUnit(board, i, Units::COL))
        {
            recheckGroups = true;
        }
    }

    return recheckGroups;
}

bool NakedTriples::checkNakedTripleInUnit(Board &board, int index, Units unit)
{
    int s = 0, nextTime = 0, previousNextTime = 0, markupsAmount = 0;
    Square *current = nullptr;
    Square *d1 = nullptr, *d2 = nullptr, *d3 = nullptr;

        // Start algorithm
        while (s < 9)
        {
            if (unit == Units::ROW)
                current = &board._squares[index][s];
            if (unit == Units::COL)
                current = &board._squares[s][index];

            markupsAmount = current->amountOfMarkups();
            if (markupsAmount == 2 || markupsAmount == 3)
            {
                if (d1 == nullptr) // Found one candidate
                {
                    d1 = current;
                }
                else if  (d1 != nullptr && d2 == nullptr && NakedTriples::areThreeTotalMarkups(d1, current)) // Found second candidate
                {
                    d2 = current;
                }
                else if (d1 != nullptr && d2 != nullptr && NakedTriples::areThreeTotalMarkups(d2, current)
                         && NakedTriples::areThreeTotalMarkups(d1, current)) // Found third candidate
                {
                    //remove all markups in that row
                    d3 = current;
                    if (unit == Units::ROW && removeTripleMarkupsInRow(board, d1, d2, d3))
                        return true;
                    if (unit == Units::COL && removeTripleMarkupsInCol(board, d1, d2, d3))
                        return true;

                }
                else // Lets make that candidate as "d1" in case the already found candidates fail
                {
                    nextTime = s;
                }
            }
            s++;
            if (s == 9 && nextTime != 0 && previousNextTime != nextTime)
            {
                previousNextTime = nextTime;
                s = nextTime;
                d1 = nullptr; // Reset candidates
                d2 = nullptr;
            }
        }
    return false;
}

bool NakedTriples::removeTripleMarkupsInCol(Board &board, Square *d1, Square *d2, Square *d3)
{
    bool wasRemoved = false;
    int col = d1->getCol();
    for (int row = 0; row < 9; row++)
    {
        for (int d = 1; d < 10; d++)
        {
            if (board._squares[row][col].digitMarkupExists(d)
                    && (d1->digitMarkupExists(d) ||  d2->digitMarkupExists(d) ||  d3->digitMarkupExists(d))
                    && row != d1->getRow() && row != d2->getRow() && row != d3->getRow())
            {
                board.setBoardMarkup(row, col, d, true);
                wasRemoved = true;
            }
        }
    }
    if (removeTripleMarkupsInSquare(board, d1, d2, d3))
    {
        wasRemoved = true;
    }
    return wasRemoved;
}

bool NakedTriples::removeTripleMarkupsInSquare(Board &board, Square *d1, Square *d2, Square *d3)
{
    int squareGroupRow = d1->getRow() / 3;
    int squareGroupCol = d1->getCol() / 3;
    bool wasRemoved = false;

    if (d1->getCol() % 3 == 0 && d2->getCol() % 3 == 1 && d3->getCol() % 3 == 2)
    {
        for (int row = squareGroupRow * 3; row < squareGroupRow * 3 + 3; row++)
        {
            for (int col = squareGroupCol * 3; col < squareGroupCol * 3 + 3; col++)
            {
                for (int d = 1; d < 10; d++)
                {
                    if (board._squares[row][col].digitMarkupExists(d)
                            && (d1->digitMarkupExists(d) ||  d2->digitMarkupExists(d) ||  d3->digitMarkupExists(d))
                            && !areInSamePlace(d1, row, col) && !areInSamePlace(d2, row, col) && !areInSamePlace(d3, row, col))
                    {
                        board.setBoardMarkup(row, col, d, true);
                        wasRemoved = true;
                    }
                }
            }
        }
    }

    return wasRemoved;
}

bool NakedTriples::removeTripleMarkupsInRow(Board &board, Square *d1, Square *d2, Square *d3)
{
    bool wasRemoved = false;
    int row = d1->getRow();
    for (int col = 0; col < 9; col++)
    {
        for (int d = 1; d < 10; d++)
        {
            if (board._squares[row][col].digitMarkupExists(d)
                    && (d1->digitMarkupExists(d) ||  d2->digitMarkupExists(d) ||  d3->digitMarkupExists(d))
                    && col != d1->getCol() && col != d2->getCol() && col != d3->getCol())
            {
                board.setBoardMarkup(row, col, d, true);
                wasRemoved = true;
            }
        }
    }
    if (removeTripleMarkupsInSquare(board, d1, d2, d3))
    {
        wasRemoved = true;
    }
    return wasRemoved;
}

bool NakedTriples::areThreeTotalMarkups(const Square *s1, const Square *s2)
{
    std::unordered_set<int> markups{};

    for (int d = 1; d < 10; d++)
    {
        if (s1->digitMarkupExists(d) || s2->digitMarkupExists(d))
        {
            markups.insert(d);
        }
    }

    return markups.size() == 3;
}

bool NakedTriples::areInSamePlace(const Square *s1, int row, int col)
{
    return s1->getCol() == col && s1->getRow() == row;
}
