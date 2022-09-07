#include "uniquerectangle.h"


bool UniqueRectangle::checkForUniqueRectangle(Board &board)
{
    bool recheckGroups = false;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            // Start rec check
            if (board._squares[i][j].getDigit() == 0 && board._squares[i][j].amountOfMarkups() == 2)
            {
                int r = 0, c = 0;
                for (c = 0; c < 9 && !(board._squares[i][c].amountOfMarkups() == 2 && board._squares[i][c] == board._squares[i][j] && j != c); c++);
                for (r = 0; r < 9 && !(board._squares[r][j].amountOfMarkups() == 2 && board._squares[r][j] == board._squares[i][j] && i != r); r++);
                if (c < 9 && r < 9 && board._squares[r][c].getDigit() == 0) // Found rectangle
                {
                    board.strategiesUsed.insert("Unique Rectangle");
                    for (int d = 1; d < 10; d++)
                    {
                        if (board._squares[i][j].digitMarkupExists(d) && board._squares[r][c].digitMarkupExists(d))
                        {
                            board.setBoardMarkup(r, c, d, true);
                            recheckGroups = true;
                        }
                    }
                    if (board._squares[r][c].amountOfMarkups() == 1)
                    {
                        board.setBoardDigit(r, c, board._squares[r][c].checkSingleMarkup(), false);
                        recheckGroups = true;
                    }
                }
                if (c < 9 && NakedPairs::checkForNakedPairs(board, i, j, i, c)) { // Naked pair in the same column
                    recheckGroups = true;
                }
                if (r < 9 && NakedPairs::checkForNakedPairs(board, i, j, r, j)) { // Naked pair in the same row
                    recheckGroups = true;
                }
            }
        }
    }
    return recheckGroups;
}
