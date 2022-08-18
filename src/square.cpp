#include "square.h"



Square::Square()
{

}

Square::Square(const int &row, const int &col)
    : _row(row), _col(col)
{
    std::fill_n(_markups, 9, false);
    this->_markups[8] = {false};
}

Square::Square(const int &row, const int &col, const int &digit, const bool &isPreset)
    : _digit(digit), _isPreset(isPreset), _row(row), _col(col)
{
    this->_markups[8] = {false};
}

bool operator== (const Square &s1, const Square &s2)
{
    for (int i = 0; i < 9; i++)
    {
        if (s1._markups[i] != s2._markups[i])
        {
            return false;
        }
    }
    return true;
}

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
    std::fill_n(_markups, 9, false);
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

int Square::amountOfMarkups() const
{
    int amount = 0;

    for (int i = 0; i < 9; i++)
    {
        if (this->_markups[i])
        {
            amount++;
        }
    }
    return amount;
}

int Square::checkSingleMarkup() const
{
    for (int i = 0; i < 9; i++)
    {
        if (this->_markups[i])
        {
            return i + 1;
        }
    }
    return 0;
}
