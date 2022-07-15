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
    void generateBoard();

signals:
    void uiSetBoardDigit(const int& col, const int& row, const int& digit, const bool& isPreset);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
