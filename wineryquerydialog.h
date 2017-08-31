#ifndef WINERYQUERYDIALOG_H
#define WINERYQUERYDIALOG_H

#include "winerymodel.h"
#include "querydialog.h"
#include "winerydialog.h"
#include <QAbstractButton>


class WineryQueryDialog : public QueryDialog
{
    Q_OBJECT
public:
    WineryQueryDialog(QString wineryStr,QSqlDatabase db, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    WineryModel * wineryModel() const;

public slots:
    void on_queryLineEdit_textEdited(const QString &text);
    void on_queryComboBox_currentIndexChanged(int index);
    void doAction(QAbstractButton *button);
};

#endif // WINERYQUERYDIALOG_H