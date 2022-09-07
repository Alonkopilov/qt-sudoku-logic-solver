#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include "board.h"
#include <bits/stdc++.h>

class Board;
class Backtracking
{
public:
    static void doBacktracking(Board &board);

private:
    static bool solve(Board &board);
    static std::pair<int,int> findEmpty(Board &board);
    static void syncBoardWithGui(Board &board);
};

#endif // BACKTRACKING_H
