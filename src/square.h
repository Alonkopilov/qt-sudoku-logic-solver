#ifndef SQUARE_H
#define SQUARE_H

#include <iostream>

class Square
{
public:
    Square();
    Square(const int &row, const int &col);
    Square(const int &row, const int &col, const int &digit, const bool &isPreset);
    bool digitMarkupExists(const int& digit) const;
    int getDigit() const;
    void setDigit(const int& digit, const bool& isPreset);
    bool setMarkup(const int& digit);
    void removeMarkup(const int& digit);
    int getRow() const;
    int getCol() const;
    int amountOfMarkups() const;
    int checkSingleMarkup() const; // For convenience

    friend bool operator== (const Square &s1, const Square &s2);

private:
    bool _markups[9];
    bool _wasOnSquareBefore[9];
    int _digit;
    bool _isPreset;
    int _row;
    int _col;
};

#endif // SQUARE_H
