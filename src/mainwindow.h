#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "board.h"
#include <QMainWindow>
#include <iostream>
#include <QLabel>

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

private slots:
    void on_btnSolve_clicked();

private:
    void uiGenerateBoard();
    void cleanLayout(QLayout* layout);
    QLabel* createFinalDigit(const int& digit, const bool& isPreset);
    Ui::MainWindow *ui;
    Board sudokuBoard;
};
#endif // MAINWINDOW_H
