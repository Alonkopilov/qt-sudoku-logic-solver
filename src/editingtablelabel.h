#ifndef EDITINGTABLELABEL_H
#define EDITINGTABLELABEL_H

#include <QWidget>
#include <QLabel>
#include <QEvent>
#include <QKeyEvent>
#include <iostream>

class EditingTableLabel : public QLabel
{
    Q_OBJECT

public:
    EditingTableLabel();
    EditingTableLabel(QWidget* parent);

signals:
    void highlightLabel();
    void editLabelFromKeyValue(const QString& digitPressed);

public slots:
    void uiHighlightLabel();
    void uiEditLabelFromKeyValue(const QString& digitPressed);

protected:
    bool event(QEvent* myEvent);
    void keyPressEvent(QKeyEvent* myEvent);
};

#endif // EDITINGTABLELABEL_H
