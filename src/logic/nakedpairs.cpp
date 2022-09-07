#include "nakedpairs.h"



bool NakedPairs::checkForNakedPairs(Board &board, const int &i, const int &j, const int &i2, const int &j2)
{
    board.strategiesUsed.insert("Naked Pairs");

    std::pair<int, int> pair = board._squares[i][j].checkPairOfMarkups();
    bool recheckGroups = false;

    if (i == i2) // Naked pair in the same row
    {
        for (int c = 0; c < 9; c++)
        {
            if (c != j && c != j2)
            {
                if (board._squares[i][c].digitMarkupExists(pair.first))
                {
                    board.setBoardMarkup(i, c, pair.first, true);
                    recheckGroups = true;
                }
                if (board._squares[i][c].digitMarkupExists(pair.second))
                {
                    board.setBoardMarkup(i, c, pair.second, true);
                    recheckGroups = true;
                }
            }
        }
    }
    if (j == j2) // Naked pair in the same column
    {
        for (int r = 0; r < 9; r++)
        {
            if (r != i && r != i2) {
                if (board._squares[r][j].digitMarkupExists(pair.first))
                {
                    board.setBoardMarkup(r, j, pair.first, true);
                    recheckGroups = true;
                }
                if (board._squares[r][j].digitMarkupExists(pair.second))
                {
                    board.setBoardMarkup(r, j, pair.second, true);
                    recheckGroups = true;
                }
            }
        }
    }
    return recheckGroups;
}
