#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int arr[81] = {0,0,4,1,0,3,8,0,0,7,0,8,0,0,0,6,0,1,0,3,0,0,8,0,0,4,0,3,9,0,0,5,0,0,6,2,0,0,5,0,3,0,9,0,0,2,8,0,0,9,0,0,7,3,0,1,0,0,6,0,0,8,0,8,0,3,0,0,0,1,0,4,0,0,7,9,0,8,3,0,0};
    Board b(arr);
    bool success = QObject::connect(&b, &Board::uiSetBoardDigit, this, &MainWindow::uiSetBoardDigit);
    std::cout << success << std::endl;

    uiGenerateBoard();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::uiGenerateBoard()
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
}

void MainWindow::uiSetBoardDigit(const int &row, const int &col, const int &digit, const bool &isPreset)
{

    std::cout << "here" << std::endl;
    QGridLayout* layoutTest = (QGridLayout*)ui->glMainBoard->children()[col * 9 + row * 9];

    if ( layoutTest != NULL )
    {
        QLayoutItem* item;
        while ( ( item = layoutTest->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
    }

    QLabel* num = new QLabel(QString::number(digit));
    QFont* font = new QFont();
    font->setPointSize(30);
    num->setAlignment(Qt::AlignCenter);
    num->setFont(*font);
    layoutTest->addWidget(num);
}

