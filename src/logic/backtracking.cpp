#include "backtracking.h"

void Backtracking::doBacktracking(Board &board)
{
    Backtracking::solve(board);
    Backtracking::syncBoardWithGui(board);
}

bool Backtracking::solve(Board &board)
{
        int row = 0;
        int col = 0;
        std::pair<int,int> square = Backtracking::findEmpty(board);
        if (square.first == -1 && square.second == -1)
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
                board.isSlowSolver ? board.setBoardDigit(row, col, num, false) : board._squares[row][col].setDigit(num, false); // Set digit
                if (solve(board))
                    return true;
                board.isSlowSolver ? board.setBoardDigit(row, col, 0, false) : board._squares[row][col].setDigit(0, false); // Remove digit
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
    return std::make_pair(-1,-1);
}

void Backtracking::syncBoardWithGui(Board &board)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            emit board.uiSetBoardDigit(i, j,  board._squares[i][j].getDigit(), board._squares[i][j].isPreset());
        }
    }
}
