#include "square.h"


Square::Square()
{

}

Square::Square(const int &row, const int &col, const int &digit, const bool &isPreset)
    : _digit(digit), _isPreset(isPreset), _row(row), _col(col) {}


bool Square::digitMarkupExists(const int &digit) const
{
    return _markups[digit - 1];
}

int Square::getDigit() const
{
    return _digit;
}

void Square::setDigit(const int &digit, const bool &isPreset)
{
    _digit = digit;
    _isPreset = isPreset;
}

void Square::setMarkup(const int &digit)
{
    _markups[digit - 1] = true;
}

void Square::removeMarkup(const int &digit)
{
     _markups[digit - 1] = false;
}

int Square::getRow() const
{
    return _row;
}

int Square::getCol() const
{
    return _col;
}
