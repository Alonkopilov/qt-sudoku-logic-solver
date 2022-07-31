#include "mainwindow.h"
#include "ui_mainwindow.h"

const int EASY_SUDOKU[81] = {0,0,4,1,0,3,8,0,0,7,0,8,0,0,0,6,0,1,0,3,0,0,8,0,0,4,0,3,9,0,0,5,0,0,6,2,0,0,5,0,3,0,9,0,0,2,8,0,0,9,0,0,7,3,0,1,0,0,6,0,0,8,0,8,0,3,0,0,0,1,0,4,0,0,7,9,0,8,3,0,0};
const int MEDIUM_SUDOKU[81] = {0,0,0,3,0,7,4,0,0,9,0,0,0,0,4,0,0,8,3,7,0,0,0,0,0,6,0,8,2,0,9,0,0,6,0,0,0,0,1,2,0,0,9,0,4,0,4,0,0,3,8,0,5,0,2,0,8,6,9,0,7,0,0,0,9,0,0,0,0,0,0,0,7,5,0,0,0,0,0,0,6};
const int HARD_SUDOKU[81] = {4,0,0,6,0,8,0,0,0,9,1,0,0,3,2,8,0,6,0,8,3,0,1,0,0,0,2,0,0,0,8,0,0,0,0,0,0,0,0,1,0,0,3,0,5,5,0,8,0,7,4,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,2,0,0,0,7,0,0,9,6,4,0,3};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    QObject::connect(&this->sudokuBoard, &Board::uiSetBoardDigit, this, &MainWindow::uiSetBoardDigit);
    QObject::connect(&this->sudokuBoard, &Board::uiAddMarkup, this, &MainWindow::uiAddMarkup);
    QObject::connect(&this->sudokuBoard, &Board::uiRemoveMarkup, this, &MainWindow::uiRemoveMarkup);
    QObject::connect(&this->sudokuBoard, &QThread::finished, this, &MainWindow::finishSudoku);


    QObject::connect(this->ui->btnLoadEasy, &QAbstractButton::clicked, this, &MainWindow::loadSudoku);
    QObject::connect(this->ui->btnLoadMedium, &QAbstractButton::clicked, this, &MainWindow::loadSudoku);
    QObject::connect(this->ui->btnLoadHard, &QAbstractButton::clicked, this, &MainWindow::loadSudoku);

    uiGenerateBoard();
    ui->btnLoadEasy->setDisabled(true);
    this->sudokuBoard.initializeBoard(EASY_SUDOKU);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::uiGenerateBoard()
{
    this->cleanLayout(ui->glMainBoard);

    for (int i = 0; i < 81; i++)
    {
        QGridLayout* layout = new QGridLayout();
        int row = i / 9;
        int col = i % 9;

        for (int j = 0; j < 9; j++)
        {
            QLabel* num = new QLabel(" ");

            num->setAlignment(Qt::AlignCenter);
            num->setStyleSheet("QLabel {color: #939EAA;}");
            layout->addWidget(num, j / 3, j % 3);
        }

        ui->glMainBoard->addLayout(layout, row, col);
    }
}

void MainWindow::cleanLayout(QLayout *layout)
{
    if ( layout != NULL )
    {
        QLayoutItem* item;
        while ( ( item = layout->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
    }
}

QLabel *MainWindow::createFinalDigit(const int &digit, const bool &isPreset)
{
    QLabel* num = new QLabel(QString::number(digit));
    QString fontColor = isPreset ? "#787878" : "#4D8DEC";
    num->setStyleSheet("QLabel { color : " + fontColor + "; }");

    QFont* font = new QFont();
    font->setFamily("Rubik Light");
    font->setPointSize(30);
    font->setBold(true);
    num->setAlignment(Qt::AlignCenter);
    num->setFont(*font);

    return num;
}

void MainWindow::uiSetBoardDigit(const int &row, const int &col, const int &digit, const bool &isPreset)
{
    if (digit == 0) return;

    QGridLayout* squareLayout = (QGridLayout*)ui->glMainBoard->itemAtPosition(row, col);
    DEBUG_PRNT(Helper::string_format("setting ui digit %d at [%d,%d]", digit, row, col));

    if (squareLayout == nullptr) {
        ERR_PRNT(Helper::string_format("squareLayout is null in index [%d,%d]", row, col));
        return;
    }

    // Remove all the markup widgets from the square layout
    this->cleanLayout(squareLayout);

    // Create the final digit widget and add it to the square
    QLabel* digitLabel = this->createFinalDigit(digit, isPreset);
    squareLayout->addWidget(digitLabel);
}

void MainWindow::uiAddMarkup(const int &row, const int &col, const int &digit)
{
    QGridLayout* squareLayout = (QGridLayout*)ui->glMainBoard->itemAtPosition(row, col);
    QLabel* markup = (QLabel*)squareLayout->itemAt(digit - 1)->widget();
    markup->setText(QString::number(digit));
}

void MainWindow::uiRemoveMarkup(const int &row, const int &col, const int &digit)
{
    QGridLayout* squareLayout = (QGridLayout*)ui->glMainBoard->itemAtPosition(row, col);
    QLabel* markup = (QLabel*)squareLayout->itemAt(digit - 1)->widget();
    markup->setText(" ");
}

void MainWindow::loadSudoku()
{
     QWidget* buttonWidget = qobject_cast<QWidget*>(sender());
     QString chosenDifficulty = buttonWidget->accessibleName();
     uiGenerateBoard();

     if (chosenDifficulty == "btnLoadEasy")
     {
        this->sudokuBoard.initializeBoard(EASY_SUDOKU);
     }
     if (chosenDifficulty == "btnLoadMedium")
     {
        this->sudokuBoard.initializeBoard(MEDIUM_SUDOKU);
     }
     if (chosenDifficulty == "btnLoadHard")
     {
        this->sudokuBoard.initializeBoard(HARD_SUDOKU);
     }

     ui->btnLoadEasy->setDisabled(chosenDifficulty == "btnLoadEasy");
     ui->btnLoadMedium->setDisabled(chosenDifficulty == "btnLoadMedium");
     ui->btnLoadHard->setDisabled(chosenDifficulty == "btnLoadHard");

     ui->btnSolve->setDisabled(false);
}

void MainWindow::finishSudoku()
{
    std::cout << "--Solving Completed--" << std::endl;
    ui->btnLoadEasy->setDisabled(false);
    ui->btnLoadMedium->setDisabled(false);
    ui->btnLoadHard->setDisabled(false);
}

void MainWindow::on_btnSolve_clicked()
{
    this->sudokuBoard.toggleSlowSolve(this->ui->cbSlowSolve->isChecked());
    this->sudokuBoard.start();

    ui->btnSolve->setDisabled(true);

    ui->btnLoadEasy->setDisabled(true);
    ui->btnLoadMedium->setDisabled(true);
    ui->btnLoadHard->setDisabled(true);
}

void MainWindow::on_btnLoadSudoku_clicked()
{
    if (this->ui->btnLoadSudoku->text() == "Load Custom Sudoku")
    {
        this->ui->btnLoadSudoku->setText("Load Sudoku by difficulty");
        // Show small editable sudoku board
    }
    else
    {
        this->ui->btnLoadSudoku->setText("Load Custom Sudoku");
        // Show difficulty buttons
    }
}

