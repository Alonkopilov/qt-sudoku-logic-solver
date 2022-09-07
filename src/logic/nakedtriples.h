#ifndef NAKEDTRIPLES_H
#define NAKEDTRIPLES_H

#include "board.h"
#include <bits/stdc++.h>

enum class Units {
    ROW,
    COL,
    GROUP
};

class Board;
class NakedTriples
{
public:
    static bool checkForNakedTriples(Board &board);

private:
    static bool checkNakedTripleInUnit(Board &board, int index, Units unit);
    static bool removeTripleMarkupsInRow(Board &board, Square* d1, Square* d2, Square* d3);
    static bool removeTripleMarkupsInCol(Board &board, Square* d1, Square* d2, Square* d3);
    static bool removeTripleMarkupsInSquare(Board &board, Square* d1, Square* d2, Square* d3);
    static bool areThreeTotalMarkups(const Square *s1, const Square *s2);
    static bool areInSamePlace(const Square *s1, int i, int j);
};

#endif // NAKEDTRIPLES_H
