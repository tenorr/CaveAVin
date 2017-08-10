#ifndef DOMAINEDIALOG_H
#define DOMAINEDIALOG_H

#include "abstractmodelformdialog.h"
#include "domainemodel.h"

#include <QSqlQuery>


class DomaineDialog : public AbstractModelFormDialog
{
    Q_OBJECT
public:
    DomaineDialog(QSqlDatabase db, int selectedId=-1, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    DomaineModel *domaineModel() const;

public slots:
    void doAction(QAbstractButton *button);

};

#endif // DOMAINEDIALOG_H
