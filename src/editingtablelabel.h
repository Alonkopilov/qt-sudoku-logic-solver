#ifndef EDITINGTABLELABEL_H
#define EDITINGTABLELABEL_H

#include <QWidget>
#include <QLabel>
#include <QEvent>
#include <QKeyEvent>
#include <iostream>

#define NOT_HIGHLIGHTED_STYLING "QLabel {background-color: #212529; color: #939EAA;}"
#define HIGHLIGHTED_STYLING "QLabel {background-color: #A2C1E0;}"

class EditingTableLabel : public QLabel
{
    Q_OBJECT

public:
    EditingTableLabel();
    EditingTableLabel(QWidget* parent);
    void uiUnhighlightLabel();

signals:
    void highlightLabel();
    void editLabelFromKeyValue(const QString& digitPressed);
    void highlightNextLabel(EditingTableLabel* editLabel);
    void unhighlightPrevLabel(EditingTableLabel* editLabel);

public slots:
    void uiEditLabelFromKeyValue(const QString& digitPressed);
    void uiHighlightLabel();

protected:
    bool event(QEvent* myEvent);
    void keyPressEvent(QKeyEvent* myEvent);

private:
    bool isDigit(const QString& str);
};

#endif // EDITINGTABLELABEL_H
