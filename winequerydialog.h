#ifndef WINEQUERYDIALOG_H
#define WINEQUERYDIALOG_H

#include "querydialog.h"
#include "winemodel.h"
#include "winedialog.h"

class WineQueryDialog : public QueryDialog
{
    Q_OBJECT
public:
    WineQueryDialog(QString domaineStr, QString wineStr, QSqlDatabase db, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    WineModel * wineModel() const;

public slots:
    void on_queryLineEdit_textEdited(const QString &text);
    void on_queryLineEdit2_textEdited(const QString &text);
    void on_queryComboBox_currentIndexChanged(int index);
    void doAction(QAbstractButton *button);
};

#endif // WINEQUERYDIALOG_H
