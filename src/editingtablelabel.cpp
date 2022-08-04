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
    this->setStyleSheet("QLabel {background-color: #A2C1E0;}");
}

void EditingTableLabel::uiEditLabelFromKeyValue(const QString& digitPressed)
{
    //Set new digit
    this->setText(digitPressed);

    //Remove styling
    this->setStyleSheet("QLabel {background-color: #333333; color: #939EAA;}");
}

bool EditingTableLabel::event(QEvent *myEvent)
{
    //std::cout << myEvent->type() << std::endl;
    if (myEvent->type() == QEvent::MouseButtonRelease)
    {
        std::cout << "Click event!" << std::endl;
        emit this->highlightLabel();
        this->setFocus(Qt::FocusReason::NoFocusReason);
    }

    return QWidget::event(myEvent);
}

void EditingTableLabel::keyPressEvent(QKeyEvent *myEvent)
{
    emit this->editLabelFromKeyValue(QString(QChar(myEvent->key())));
}
