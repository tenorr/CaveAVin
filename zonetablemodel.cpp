#include "zonetablemodel.h"
#include <QDebug>

ZoneTableModel::ZoneTableModel(QObject *parent, QSqlDatabase db)
    : AbstractRectModel("Zone",parent,db)
{
    setParentName("Storage");
}

void ZoneTableModel::deleteStorageZones(int storageId)
{
    QSqlRecord rec;
    for (int i=0; i<rowCount();i++) {
     rec = record(i);
     // Delete record if the zone is assign to the Storage
     if (rec.value("Storage").toInt()==storageId)
         removeRow(i);
    }
    submitAll();
}
