#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QFontDatabase::addApplicationFont(":/fonts/Montserrat-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Montserrat-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Rubik-Light.ttf");

    w.show();
    return a.exec();
}
