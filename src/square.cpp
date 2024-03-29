#include "square.h"



Square::Square()
{

}

Square::Square(const int &row, const int &col)
    : _row(row), _col(col)
{
    std::fill_n(_markups, 9, false);
    std::fill_n(_wasOnSquareBefore, 9, false);
    this->_markups[8] = {false};
    this->_wasOnSquareBefore[8] = {false};
}

Square::Square(const int &row, const int &col, const int &digit, const bool &isPreset)
    : _digit(digit), _isPreset(isPreset), _row(row), _col(col)
{
    std::fill_n(_markups, 9, false);
    std::fill_n(_wasOnSquareBefore, 9, false);
    this->_markups[8] = {false};
    this->_wasOnSquareBefore[8] = {false};
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

bool Square::setMarkup(const int &digit)
{
    if (!this->_wasOnSquareBefore[digit - 1]) {
        _markups[digit - 1] = true;
        return true;
    }
    return false;
}

bool Square::removeMarkup(const int &digit)
{
    if (this->_markups[digit - 1]) {
        _markups[digit - 1] = false;
        _wasOnSquareBefore[digit - 1] = true;
        return true;
    }
    return false;
}

int Square::getRow() const
{
    return _row;
}

int Square::getCol() const
{
    return _col;
}

bool Square::isPreset() const
{
    return _isPreset;
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

std::pair<int, int> Square::checkPairOfMarkups() const
{
    int amount = 0;
    int d1 = 0, d2 = 0;

    for (int i = 0; i < 9; i++)
    {
        if (this->_markups[i])
        {
            amount++;
            d1 == 0 ? d1 = i : d2 = i;
        }
    }
    if (amount == 2)
    {
        return std::make_pair(d1 + 1, d2 + 1);
    }
    return std::make_pair(0, 0);
}
