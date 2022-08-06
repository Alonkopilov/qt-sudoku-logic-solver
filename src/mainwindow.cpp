#include "mainwindow.h"
#include "ui_mainwindow.h"

const int EASY_SUDOKU[81] = {0,0,4,1,0,3,8,0,0,7,0,8,0,0,0,6,0,1,0,3,0,0,8,0,0,4,0,3,9,0,0,5,0,0,6,2,0,0,5,0,3,0,9,0,0,2,8,0,0,9,0,0,7,3,0,1,0,0,6,0,0,8,0,8,0,3,0,0,0,1,0,4,0,0,7,9,0,8,3,0,0};
const int MEDIUM_SUDOKU[81] = {0,0,0,3,0,7,4,0,0,9,0,0,0,0,4,0,0,8,3,7,0,0,0,0,0,6,0,8,2,0,9,0,0,6,0,0,0,0,1,2,0,0,9,0,4,0,4,0,0,3,8,0,5,0,2,0,8,6,9,0,7,0,0,0,9,0,0,0,0,0,0,0,7,5,0,0,0,0,0,0,6};
const int HARD_SUDOKU[81] = {4,0,0,6,0,8,0,0,0,9,1,0,0,3,2,8,0,6,0,8,3,0,1,0,0,0,2,0,0,0,8,0,0,0,0,0,0,0,0,1,0,0,3,0,5,5,0,8,0,7,4,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,2,0,0,0,7,0,0,9,6,4,0,3};
//const int COMP_SUDOKU[81] = {0, 4, 2, 6, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 7, 0, 0, 5, 3, 8, 0, 0, 1, 6, 0, 2, 0, 0, 5, 0, 0, 9, 0, 7, 0, 0, 0, 0, 8, 0, 6, 0, 0, 0,0, 2, 0, 7, 0, 0, 1, 0, 0, 8, 0, 4, 1, 0, 0, 2, 3, 6, 0, 0, 1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2, 8, 9, 0};

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
    uiGenerateEditBoard();
    setEditBoardVisibility(false);

    ui->btnLoadEasy->setDisabled(true);
    this->lastHighlighted = nullptr;
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

void MainWindow::uiGenerateEditBoard()
{
    for (int i = 0; i < 81; i++)
    {
        int row = i / 9;
        int col = i % 9;

        EditingTableLabel* num = new EditingTableLabel();
        QObject::connect(num, &EditingTableLabel::highlightNextLabel, this, &MainWindow::uiFocusNextEditingSquare);
        QObject::connect(num, &EditingTableLabel::unhighlightPrevLabel, this, &MainWindow::uiRemoveFocusFromPrevEditingSquare);
        num->setObjectName(QString::fromStdString("lEdit" + std::to_string(i)));
        num->resize(50, 50);
        num->setStyleSheet("QLabel {color: #939EAA;}");
        num->setText(" ");
        num->setAlignment(Qt::AlignCenter);

        QFont* font = new QFont();
        font->setFamily("Rubik Light");
        font->setPointSize(20);

        num->setFont(*font);
        ui->glEditBoard->addWidget(num, row, col);
    }
}

void MainWindow::setEditBoardVisibility(const bool &isVisible)
{
    QRegularExpression exp("line_2[2-9]|line_3[0-9]|line_4[0-2]"); // Range of line_22 to line_42 -> edit board line IDs
    QList<QWidget*> editBoardLines = ui->centralwidget->findChildren<QWidget*>(exp);
    for (int i = 0; i < editBoardLines.size(); i++)
    {
        editBoardLines[i]->setVisible(isVisible);
    }

    for (int i = 0; i < 81; i++)
    {
        int row = i / 9;
        int col = i % 9;
        QLabel* editSquare = (QLabel*)ui->glEditBoard->itemAtPosition(row, col);

        editSquare->setVisible(isVisible);
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

void MainWindow::uiFocusNextEditingSquare(EditingTableLabel* editLabel)
{
    int index = editLabel->objectName().mid(5,2).toInt();

    if (index < 80)
    {
        EditingTableLabel* nextLabel = (EditingTableLabel*)ui->glEditBoard->itemAt(index + 1)->widget();
        this->lastHighlighted = nextLabel;
        nextLabel->setFocus();
        nextLabel->uiHighlightLabel();
    }
}

void MainWindow::uiRemoveFocusFromPrevEditingSquare(EditingTableLabel* editLabel)
{
    if (this->lastHighlighted != nullptr)
    {
        this->lastHighlighted->uiUnhighlightLabel();
    }
    this->lastHighlighted = editLabel;
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
    if (this->ui->btnSolve->text() == "LOAD TO MAIN BOARD")
    {
        uiGenerateBoard();
        transferFromEditingToMainBoard();
    }

    this->sudokuBoard.toggleSlowSolve(this->ui->cbSlowSolve->isChecked());
    this->sudokuBoard.start();
    ui->btnSolve->setDisabled(true);

    ui->btnLoadEasy->setDisabled(true);
    ui->btnLoadMedium->setDisabled(true);
    ui->btnLoadHard->setDisabled(true);

}

void MainWindow::showDiffLoad()
{
    this->ui->btnLoadEasy->setVisible(true);
    this->ui->btnLoadMedium->setVisible(true);
    this->ui->btnLoadHard->setVisible(true);
    setEditBoardVisibility(false);
}

void MainWindow::showCustomLoad()
{
    this->ui->btnLoadEasy->setVisible(false);
    this->ui->btnLoadMedium->setVisible(false);
    this->ui->btnLoadHard->setVisible(false);
    setEditBoardVisibility(true);
}

void MainWindow::on_btnLoadByCustomDiff_clicked()
{
    if (this->ui->btnLoadByCustomDiff->text() == "Load Custom Sudoku")    //If the program is in "Difficulty Load" mode, switch to "Custom Load" mode
    {
        this->ui->btnLoadByCustomDiff->setText("Load Sudoku by difficulty");
        showCustomLoad();
        this->ui->btnSolve->setText("LOAD TO MAIN BOARD");
        QObject::connect(this->ui->btnSolve, &QAbstractButton::clicked, this, &MainWindow::on_btnSolve_clicked);
    }
    else                                                                //If the program is in "Custom Load" mode, switch to "Difficulty Load" mode
    {
        this->ui->btnLoadByCustomDiff->setText("Load Custom Sudoku");
        showDiffLoad();
        this->ui->btnSolve->setText("SOLVE");
        QObject::connect(this->ui->btnSolve, &QAbstractButton::clicked, this, &MainWindow::on_btnSolve_clicked);
    }
}

void MainWindow::transferFromEditingToMainBoard()
{
    int arr[81] = {0};
    QString curr = nullptr;

    for (int i = 0; i < this->ui->glEditBoard->count(); i++)
    {
        curr = ((EditingTableLabel*)this->ui->glEditBoard->itemAt(i)->widget())->text();
        curr == " " ? arr[i] = 0 : arr[i] = curr.toInt();
    }

    this->sudokuBoard.initializeBoard(arr);
}

