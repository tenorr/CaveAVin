#ifndef WINERYDIALOG_H
#define WINERYDIALOG_H

#include "abstractmodelformdialog.h"
#include "winerymodel.h"

#include <QSqlQuery>


class WineryDialog : public AbstractModelFormDialog
{
    Q_OBJECT
public:
    WineryDialog(QSqlDatabase db, int selectedId=-1, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    WineryModel *wineryModel() const;

public slots:
    void doAction(QAbstractButton *button);

};

#endif // WINERYDIALOG_H