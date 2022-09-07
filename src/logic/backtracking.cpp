#include "backtracking.h"

void Backtracking::doBacktracking(Board &board)
{
    Backtracking::solve(board);
}

bool Backtracking::solve(Board &board)
{
        int row = 0;
        int col = 0;
        std::pair<int,int> square = Backtracking::findEmpty(board);
        if (square.first == 0 && square.second == 0)
        {
            return true;
        }
        else
        {
            row = square.first;
            col = square.second;
        }

        for (int num = 1; num <= 9; num++)
        {
            if (board.checkSafe(board._squares[row][col], num))
            {
                board.setBoardDigit(row, col, num, false);
                if (solve(board))
                    return true;
                board._squares[row][col].setDigit(0, false); // Using setBoardDigit will notify and UI and will slow down the program considerably
            }
        }
        return false;
}

std::pair<int,int> Backtracking::findEmpty(Board &board)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (board.getBoardDigit(i, j) == 0)
            {
                return std::make_pair(i, j);
            }
        }
    }
    return std::make_pair(0,0);
}
