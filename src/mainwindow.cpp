#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QLabel"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev);
    QMetaObject::invokeMethod(this, "afterWindowShown", Qt::ConnectionType::QueuedConnection);
}

void MainWindow::generateBoard()
{
    for (int i = 0; i < 81; i++)
    {
        std::cout << "Hello" << std::endl;
        QLayout* sqaureMarkupsLayout = new QGridLayout();

        for (int j = 0; j < 9; j++)
        {
            QLabel* markup = new QLabel("1");
            sqaureMarkupsLayout->addWidget(markup);
        }

        ui->gridLayout->addLayout(sqaureMarkupsLayout,);
    }
}

void MainWindow::afterWindowShown()
{
    std::cout << "START" << std::endl;
    generateBoard();
    // Hide markups
    //for (int i = 0; i < ui->gridLayout_2->count(); ++i) {
    //    QWidget *w = ui->gridLayout_2->itemAt(i)->widget();
    //    if(w != NULL)
    //        w->hide();
    //}

    // Delete all markups
    //while (ui->gridLayout_2->takeAt(0) != NULL)
    //{
    //    delete ui->gridLayout_2->takeAt(0)->widget();
    //    delete ui->gridLayout_2->takeAt(0);
    //}

    //// Show final digit
    //QLabel* wid = new QLabel("1");
    //ui->gridLayout_2->addWidget(wid);
}

