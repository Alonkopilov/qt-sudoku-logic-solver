#include "editingtablelabel.h"

EditingTableLabel::EditingTableLabel()
{
    QObject::connect(this, &EditingTableLabel::highlightLabel, this, &EditingTableLabel::uiHighlightLabel);
    QObject::connect(this, &EditingTableLabel::editLabelFromKeyValue, this, &EditingTableLabel::uiEditLabelFromKeyValue);
}

EditingTableLabel::EditingTableLabel(QWidget *parent) : QLabel(parent)
{
    QObject::connect(this, &EditingTableLabel::highlightLabel, this, &EditingTableLabel::uiHighlightLabel);
    QObject::connect(this, &EditingTableLabel::editLabelFromKeyValue, this, &EditingTableLabel::uiEditLabelFromKeyValue);
}

void EditingTableLabel::uiUnhighlightLabel()
{
    //Remove styling
    this->setStyleSheet(NOT_HIGHLIGHTED_STYLING);
}

void EditingTableLabel::uiHighlightLabel()
{
    //Set styling
    this->setStyleSheet(HIGHLIGHTED_STYLING);
}

void EditingTableLabel::uiEditLabelFromKeyValue(const QString& digitPressed)
{
    //Set new digit
    digitPressed == "0" ? this->setText(" ") : this->setText(digitPressed);

    //Remove styling
    this->uiUnhighlightLabel();

    //Set focus to next square
    emit this->highlightNextLabel(this);
}

bool EditingTableLabel::event(QEvent *myEvent)
{
    if (myEvent->type() == QEvent::MouseButtonPress)
    {
        std::cout << "Click event!" << std::endl;
        emit this->highlightLabel();
        emit this->unhighlightPrevLabel(this);
        this->setFocus();
    }

    return QWidget::event(myEvent);
}

void EditingTableLabel::keyPressEvent(QKeyEvent *myEvent)
{
    if (isDigit(myEvent->text()))
    {
        emit this->editLabelFromKeyValue(QString(QChar(myEvent->key())));
    }
}

bool EditingTableLabel::isDigit(const QString &str)
{
    return std::isdigit(str.toStdString()[0]);
}
