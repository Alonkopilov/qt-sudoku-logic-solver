#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Board b({});
    //QObject::connect(this, &MainWindow::uiSetBoardDigit, &b, &Board::setBoardDigit);

    generateBoard();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generateBoard()
{
    for (int i = 0; i < ui->glMainBoard->children().length(); i++)
    {
        QGridLayout* layout = (QGridLayout*)ui->glMainBoard->children()[i];

        for (int j = 0; j < 9; j++) {
            QLabel* num = new QLabel("0");
            num->setAlignment(Qt::AlignCenter);
            num->setStyleSheet("QLabel {color: #B4BBC4;}");

            layout->addWidget(num, j/3, j%3);
        }

    }

    //TESTING
    QGridLayout* layoutTest = (QGridLayout*)ui->glMainBoard->children()[2];

    if ( layoutTest != NULL )
    {
        QLayoutItem* item;
        while ( ( item = layoutTest->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
    }

    QLabel* num = new QLabel("0");
    QFont* font = new QFont();
    font->setPointSize(30);
    num->setAlignment(Qt::AlignCenter);
    num->setFont(*font);
    layoutTest->addWidget(num);
}

