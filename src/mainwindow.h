#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "board.h"
#include "utils/helper.h"
#include "editingtablelabel.h"
#include <QMainWindow>
#include <iostream>
#include <QLabel>
#include <QLine>
#include <QRegularExpression>

#ifdef DEBUG
#define DEBUG_PRNT(str) do { std::cout << "[debug] " + str << std::endl; } while( false )
#define ERR_PRNT(str) do { std::cout << "[error] " + str << std::endl; } while( false )
#else
#define DEBUG_PRNT(str) do { } while ( false )
#define ERR_PRNT(str) do { } while ( false )
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define LOG_ERR_STYLESHEET "QLabel {font: 600 10pt \"Montserrat\"; color: #FF4235;}"
#define LOG_MSG_STYLESHEET "QLabel {font: 600 10pt \"Montserrat\"; color: #6DC43E;}"
#define EASY_SUDOKU_BTN_STYLESHEET ":enabled { background-color: #2C2C2C; border: 1px solid #2188e1; color: rgb(255, 255, 255); font: 12pt \"Montserrat\"; } :disabled { background-color: rgba(255, 255, 255, 0); border: 1px solid #505050; color: #505050; font: 12pt \"Montserrat\"; }"
#define MEDIUM_SUDOKU_BTN_STYLESHEET ":enabled { background-color: #2C2C2C; border: 1px solid #fd7e14; color: rgb(255, 255, 255); font: 12pt \"Montserrat\"; } :disabled { background-color: rgba(255, 255, 255, 0); border: 1px solid #505050; color: #505050; font: 12pt \"Montserrat\"; }"
#define HARD_SUDOKU_BTN_STYLESHEET ":enabled { background-color: #2C2C2C; border: 1px solid #fa5252; color: rgb(255, 255, 255); font: 12pt \"Montserrat\"; } :disabled { background-color: rgba(255, 255, 255, 0); border: 1px solid #505050; color: #505050; font: 12pt \"Montserrat\"; }"
#define MARKUP_LABEL_STYLESHEET "QLabel {color: #939EAA;}"
#define PRESET_FINAL_DIGIT_STYLESHEET "QLabel {color: #939EAA;}"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadSudoku();


public slots:
    void uiSetBoardDigit(const int &row, const int &col, const int &digit, const bool &isPreset);
    void uiAddMarkup(const int& row, const int& col, const int& digit);
    void uiRemoveMarkup(const int& row, const int& col, const int& digit);
    void uiFocusNextEditingSquare(EditingTableLabel* editLabel);
    void uiRemoveFocusFromPrevEditingSquare(EditingTableLabel* editLabel);
    void uiWriteToLog(const QString& str, const bool& isErr);
    void uiWriteToStrategiesLabel(const QString& str);
    void solveOrLoadBoard();
    void loadByCustomOrByDiff();

private:
    void enableDifficultyButtons();
    void transferFromEditingToMainBoard();
    void showDiffLoad();
    void showCustomLoad();
    void uiGenerateBoard();
    void uiGenerateEditBoard();
    void setEditBoardVisibility(const bool& isVisible);
    void cleanLayout(QLayout* layout);
    QLabel* createFinalDigit(const int& digit, const bool& isPreset);

    Ui::MainWindow *ui;
    EditingTableLabel* lastHighlighted;
    Board sudokuBoard;
};
#endif // MAINWINDOW_H
