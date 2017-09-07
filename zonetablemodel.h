#ifndef ZONETABLEMODEL_H
#define ZONETABLEMODEL_H

#include "abstractrectmodel.h"

class ZoneTableModel : public AbstractRectModel
{
    Q_OBJECT
public:
    ZoneTableModel(QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());

    void deleteStorageZones(int storageId);
};

#endif // ZONETABLEMODEL_H
