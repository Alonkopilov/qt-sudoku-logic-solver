#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    int res2 = QFontDatabase::addApplicationFont(":/fonts/fonts/Montserrat-Bold.ttf");
    int res1 = QFontDatabase::addApplicationFont(":/fonts/fonts/Montserrat-Regular.ttf");



    QFontDatabase db;
     for(int i=0; i<db.families().size(); i++)
     {
       qDebug() << db.families().at(i);
     }
     std::cout << "res1 = " + std::to_string(res1) + ", res2 = " + std::to_string(res2) << std::endl;

    w.show();
    return a.exec();
}
