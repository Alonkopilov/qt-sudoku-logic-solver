#include "hiddensingles.h"


bool HiddenSingles::checkForHiddenSingles(Board &board)
{
    bool recheckGroups = false;

    for (int groupRow = 0; groupRow < 3; groupRow++)
    {
        for (int groupCol = 0; groupCol < 3; groupCol++)
        {
            if (checkSquareGroup(board, groupRow, groupCol))
                recheckGroups = true;
        }
    }
    return recheckGroups;
}

bool HiddenSingles::checkSquareGroup(Board &board, const int &squareGroupRow, const int &squareGroupCol)
{
    int markupAmount = 0;
    int markupAmountOnSquareGroup = 0;
    int markupPosition = 0;
    bool recheckGroups = false;

    // Go through all digits
    for (int d = 1; d < 10; d++)
    {
        markupAmountOnSquareGroup = 0;
        for (int i = squareGroupRow * 3; i < squareGroupRow * 3 + 3; i++)
        {
            for (int j = squareGroupCol * 3; j < squareGroupCol * 3 + 3; j++)
            {
                if (board.getBoardDigit(i, j) == 0 && board._squares[i][j].digitMarkupExists(d))
                {
                    markupAmountOnSquareGroup++;
                    markupPosition = i * 10 + j;

                    markupAmount = 0;
                    for (int m = 0; m < 9; m++) // Check row
                    {
                        if (board.getBoardDigit(i, m) == 0 && board._squares[i][m].digitMarkupExists(d))
                        {
                            markupAmount++;
                        }
                    }
                    if (markupAmount == 1 && board.checkSafe(board._squares[i][j], d))
                    {
                        board.setBoardDigit(i, j, d, false);
                        recheckGroups = true;
                    }

                    markupAmount = 0;
                    for (int m = 0; m < 9; m++) // Check column
                    {
                        if (board.getBoardDigit(m, j) == 0 && board._squares[m][j].digitMarkupExists(d))
                        {
                             markupAmount++;
                        }
                    }
                    if (markupAmount == 1 && board.checkSafe(board._squares[i][j], d))
                    {
                        board.setBoardDigit(i, j, d, false);
                        recheckGroups = true;
                    }

                }
            }
        }
        if (markupAmountOnSquareGroup == 1 && board.checkSafe(board._squares[markupPosition / 10][markupPosition % 10], d)) // Check square group
        {
            board.setBoardDigit(markupPosition / 10, markupPosition % 10, d, false);
            recheckGroups = true;
        }
    }
    return recheckGroups;
}
