#ifndef DOMAINEQUERYDIALOG_H
#define DOMAINEQUERYDIALOG_H

#include "domainemodel.h"
#include "querydialog.h"
#include "domainedialog.h"
#include <QAbstractButton>


class DomaineQueryDialog : public QueryDialog
{
    Q_OBJECT
public:
    DomaineQueryDialog(QString domaineStr,QSqlDatabase db, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    DomaineModel * domaineModel() const;

public slots:
    void on_queryLineEdit_textEdited(const QString &text);
    void on_queryComboBox_currentIndexChanged(int index);
    void doAction(QAbstractButton *button);
};

#endif // DOMAINEQUERYDIALOG_H
