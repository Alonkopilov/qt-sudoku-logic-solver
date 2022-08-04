#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "board.h"
#include "helper.h"
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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void uiSetBoardDigit(const int &row, const int &col, const int &digit, const bool &isPreset);
    void uiAddMarkup(const int& row, const int& col, const int& digit);
    void uiRemoveMarkup(const int& row, const int& col, const int& digit);
    void loadSudoku();
    void finishSudoku();
    void on_btnSolve_clicked();
    void on_btnLoadSudoku_clicked();

private slots:
    void on_btnLoadByCustomDiff_clicked();

private:
    void showDiffLoad();
    void showCustomLoad();

    void uiGenerateBoard();
    void uiGenerateEditBoard();
    void setEditBoardVisibility(const bool& isVisible);
    void cleanLayout(QLayout* layout);
    QLabel* createFinalDigit(const int& digit, const bool& isPreset);
    Ui::MainWindow *ui;
    Board sudokuBoard;
};
#endif // MAINWINDOW_H
