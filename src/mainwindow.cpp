#include "mainwindow.h"
#include "ui_mainwindow.h"

const int EASY_SUDOKU[81] = {0,0,0,3,0,7,4,0,0,9,0,0,0,0,4,0,0,8,3,7,0,0,0,0,0,6,0,8,2,0,9,0,0,6,0,0,0,0,1,2,0,0,9,0,4,0,4,0,0,3,8,0,5,0,2,0,8,6,9,0,7,0,0,0,9,0,0,0,0,0,0,0,7,5,0,0,0,0,0,0,6};
const int MEDIUM_SUDOKU[81] = {0,0,4,1,0,3,8,0,0,7,0,8,0,0,0,6,0,1,0,3,0,0,8,0,0,4,0,3,9,0,0,5,0,0,6,2,0,0,5,0,3,0,9,0,0,2,8,0,0,9,0,0,7,3,0,1,0,0,6,0,0,8,0,8,0,3,0,0,0,1,0,4,0,0,7,9,0,8,3,0,0};
const int HARD_SUDOKU[81] = {8,6,0,0,0,3,0,0,0,0,0,0,5,0,9,1,0,2,0,0,0,0,4,0,0,0,0,2,0,6,0,0,0,3,0,5,0,0,0,0,0,0,7,0,0,0,4,0,0,0,0,0,0,1,4,3,2,0,0,0,0,0,7,0,1,0,6,0,8,0,0,0,0,0,0,0,0,2,0,0,0};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    QObject::connect(&this->sudokuBoard, &Board::uiSetBoardDigit, this, &MainWindow::uiSetBoardDigit);
    QObject::connect(&this->sudokuBoard, &Board::uiAddMarkup, this, &MainWindow::uiAddMarkup);
    QObject::connect(&this->sudokuBoard, &Board::uiRemoveMarkup, this, &MainWindow::uiRemoveMarkup);
    QObject::connect(&this->sudokuBoard, &Board::uiWriteToLog, this, &MainWindow::uiWriteToLog);
    QObject::connect(&this->sudokuBoard, &Board::uiWriteToStrategiesLabel, this, &MainWindow::uiWriteToStrategiesLabel);
    QObject::connect(&this->sudokuBoard, &QThread::finished, this, &MainWindow::enableDifficultyButtons);

    QObject::connect(this->ui->btnLoadEasy, &QAbstractButton::clicked, this, &MainWindow::loadSudoku);
    QObject::connect(this->ui->btnLoadMedium, &QAbstractButton::clicked, this, &MainWindow::loadSudoku);
    QObject::connect(this->ui->btnLoadHard, &QAbstractButton::clicked, this, &MainWindow::loadSudoku);
    QObject::connect(this->ui->btnSolve, &QAbstractButton::clicked, this, &MainWindow::solveOrLoadBoard);
    QObject::connect(this->ui->btnLoadByCustomDiff, &QAbstractButton::clicked, this, &MainWindow::loadByCustomOrByDiff);

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
            num->setStyleSheet(PRESET_FINAL_DIGIT_STYLESHEET);
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
    QString findEditingBoardLinesRegex = "line_2[3-9]|line_3[0-9]|line_4[0-2]"; // Range of line_23 to line_42 -> editboard line IDs
    QRegularExpression exp(findEditingBoardLinesRegex);

    QList<QWidget*> editBoardLines = ui->centralwidget->findChildren<QWidget*>(exp);
    for (int i = 0; i < editBoardLines.size(); i++)
    {
        editBoardLines[i]->setVisible(isVisible);
    }
    for (int i = 0; i < 81; i++)
    {
        ui->glEditBoard->itemAt(i)->widget()->setVisible(isVisible);
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
    QLabel* num = new QLabel(digit != 0 ? QString::number(digit): " ");
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
    QGridLayout* squareLayout = (QGridLayout*)ui->glMainBoard->itemAtPosition(row, col);
    DEBUG_PRNT(Helper::string_format("setting ui digit %d at [%d,%d]", digit, row, col));

    if (squareLayout == nullptr) {
        ERR_PRNT(Helper::string_format("squareLayout is null in index [%d,%d]", row, col));
        return;
    }

    if (digit != 0) {
        // Remove all the markup widgets from the square layout
        this->cleanLayout(squareLayout);

        // Create the final digit widget and add it to the square
        QLabel* digitLabel = this->createFinalDigit(digit, isPreset);
        squareLayout->addWidget(digitLabel);
    }
    else {
        // Remove all the markup widgets from the square layout
        this->cleanLayout(squareLayout);

        for (int j = 0; j < 9; j++)
        {
            QLabel* num = new QLabel(" ");

            num->setAlignment(Qt::AlignCenter);
            num->setStyleSheet(MARKUP_LABEL_STYLESHEET);
            squareLayout->addWidget(num, j / 3, j % 3);
        }

    }
}

void MainWindow::uiAddMarkup(const int &row, const int &col, const int &digit)
{
    //std::cout << "Markup added [" + std::to_string(row) + ", " + std::to_string(col) + "] d=" + std::to_string(digit) << std::endl;
    QGridLayout* squareLayout = (QGridLayout*)ui->glMainBoard->itemAtPosition(row, col);
    QLabel* markup = (QLabel*)squareLayout->itemAt(digit - 1)->widget();
    markup->setText(QString::number(digit));
}

void MainWindow::uiRemoveMarkup(const int &row, const int &col, const int &digit)
{
    //std::cout << "Markup removed [" + std::to_string(row) + ", " + std::to_string(col) + "] d=" + std::to_string(digit) << std::endl;
    QGridLayout* squareLayout = (QGridLayout*)ui->glMainBoard->itemAtPosition(row, col);
    QLabel* markup = (QLabel*)squareLayout->itemAt(digit - 1)->widget();
    markup->setText(" ");
}

void MainWindow::uiFocusNextEditingSquare(EditingTableLabel* editLabel)
{
    int index = editLabel->objectName().mid(5,2).toInt();

    this->ui->btnSolve->setEnabled(true);
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
    if (this->lastHighlighted != nullptr && editLabel->objectName() != this->lastHighlighted->objectName())
    {
        this->lastHighlighted->uiUnhighlightLabel();
    }
    this->lastHighlighted = editLabel;
}

void MainWindow::uiWriteToLog(const QString& str, const bool& isErr)
{
    isErr ? this->ui->lLog->setStyleSheet(LOG_ERR_STYLESHEET) : this->ui->lLog->setStyleSheet(LOG_MSG_STYLESHEET);
    this->ui->lLog->setText(str);
    this->sudokuBoard.toggleSlowSolve(false);
}

void MainWindow::uiWriteToStrategiesLabel(const QString &str)
{
    this->ui->lStrategiesUsed->setText(str);
}

void MainWindow::loadSudoku()
{
     QWidget* buttonWidget = qobject_cast<QWidget*>(sender());
     QString chosenDifficulty = buttonWidget->accessibleName();

     if (chosenDifficulty == "btnLoadEasy")
     {
        this->sudokuBoard.initializeBoard(EASY_SUDOKU);
        this->ui->btnSolve->setStyleSheet(EASY_SUDOKU_BTN_STYLESHEET);
     }
     if (chosenDifficulty == "btnLoadMedium")
     {
        this->sudokuBoard.initializeBoard(MEDIUM_SUDOKU);
        this->ui->btnSolve->setStyleSheet(MEDIUM_SUDOKU_BTN_STYLESHEET);

     }
     if (chosenDifficulty == "btnLoadHard")
     {
        this->sudokuBoard.initializeBoard(HARD_SUDOKU);
        this->ui->btnSolve->setStyleSheet(HARD_SUDOKU_BTN_STYLESHEET);
     }

     ui->btnLoadEasy->setDisabled(chosenDifficulty == "btnLoadEasy");
     ui->btnLoadMedium->setDisabled(chosenDifficulty == "btnLoadMedium");
     ui->btnLoadHard->setDisabled(chosenDifficulty == "btnLoadHard");

     ui->btnSolve->setDisabled(false);
}

void MainWindow::enableDifficultyButtons()
{
    ui->btnLoadEasy->setDisabled(false);
    ui->btnLoadMedium->setDisabled(false);
    ui->btnLoadHard->setDisabled(false);
}

void MainWindow::solveOrLoadBoard()
{
    if (this->ui->btnSolve->text() == "LOAD TO MAIN BOARD")
    {
        transferFromEditingToMainBoard();
    }

    this->ui->lStrategiesUsed->setText("Solved using: ");
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

void MainWindow::loadByCustomOrByDiff()
{
    if (this->lastHighlighted != nullptr)
        this->lastHighlighted->uiUnhighlightLabel();
    if (this->ui->btnLoadByCustomDiff->text() == "Load Custom Sudoku")    //If the program is in "Difficulty Load" mode, switch to "Custom Load" mode
    {
        this->ui->btnLoadByCustomDiff->setText("Load Sudoku by difficulty");
        showCustomLoad();
        this->ui->btnSolve->setText("LOAD TO MAIN BOARD");
    }
    else                                                                //If the program is in "Custom Load" mode, switch to "Difficulty Load" mode
    {
        this->ui->btnLoadByCustomDiff->setText("Load Custom Sudoku");
        showDiffLoad();
        this->ui->btnSolve->setText("SOLVE");
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

