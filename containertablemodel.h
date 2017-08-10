#ifndef CONTAINERTABLEMODEL_H
#define CONTAINERTABLEMODEL_H


#include "abstractrectmodel.h"

class ContainerTableModel : public  AbstractRectModel
{
    Q_OBJECT
public:
    ContainerTableModel(QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());
};

#endif // CONTAINERTABLEMODEL_H
