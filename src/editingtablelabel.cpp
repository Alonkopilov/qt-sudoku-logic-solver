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

void EditingTableLabel::uiHighlightLabel()
{
    //Set styling
    this->setStyleSheet("QLabel {background-color: #A2C1E0;}");

}

void EditingTableLabel::uiEditLabelFromKeyValue(const QString& digitPressed)
{
    //Check character validity
    if (!std::isdigit(digitPressed.toStdString()[0])) return;

    //Set new digit
    digitPressed == "0" ? this->setText(" ") : this->setText(digitPressed);

    //Remove styling
    this->setStyleSheet("QLabel {background-color: #333333; color: #939EAA;}");

    //Set focus to next square
    emit this->highlightNextLabel(this);
}

bool EditingTableLabel::event(QEvent *myEvent)
{
    if (myEvent->type() == QEvent::MouseButtonPress)
    {
        std::cout << "Click event!" << std::endl;
        emit this->highlightLabel();
        this->setFocus();
    }

    return QWidget::event(myEvent);
}

void EditingTableLabel::keyPressEvent(QKeyEvent *myEvent)
{
    emit this->editLabelFromKeyValue(QString(QChar(myEvent->key())));
}
