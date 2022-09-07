#ifndef HIDDENSINGLES_H
#define HIDDENSINGLES_H

#include "board.h"
#include <bits/stdc++.h>

class Board;
class HiddenSingles
{
public:
    static bool checkForHiddenSingles(Board &board);

private:
    static bool checkSquareGroup(Board &board, const int& squareGroupRow, const int& squareGroupCol);
};

#endif // HIDDENSINGLES_H
