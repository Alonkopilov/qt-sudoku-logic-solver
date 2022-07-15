#ifndef SQUARE_H
#define SQUARE_H


class Square
{
public:
    Square();
    Square(const int &row, const int &col, const int &digit, const bool &isPreset);
    bool digitMarkupExists(const int& digit) const;
    int getDigit() const;
    void setDigit(const int& digit, const bool& isPreset);
    void setMarkup(const int& digit);
    void removeMarkup(const int& digit);
    int getRow() const;
    int getCol() const;

private:
    bool _markups[9];
    int _digit;
    bool _isPreset;
    int _row;
    int _col;
};

#endif // SQUARE_H
