#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <iostream>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showEvent(QShowEvent* ev);

    void generateBoard();

private:
    Ui::MainWindow *ui;
private slots:
    void afterWindowShown();
};
#endif // MAINWINDOW_H
