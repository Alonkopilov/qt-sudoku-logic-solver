#ifndef NAKEDPAIRS_H
#define NAKEDPAIRS_H

#include "board.h"
#include <bits/stdc++.h>

class Board;
class NakedPairs
{
public:
    static bool checkForNakedPairs(Board &board, const int &i, const int &j, const int &i2, const int &j2);
};

#endif // NAKEDPAIRS_H
