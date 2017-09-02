#include "zonetablemodel.h"
#include <QDebug>

ZoneTableModel::ZoneTableModel(QObject *parent, QSqlDatabase db)
    : AbstractRectModel("Zone",parent,db)
{
    setParentName("Container");
}

void ZoneTableModel::deleteContainerZones(int containerId)
{
    QSqlRecord rec;
    for (int i=0; i<rowCount();i++) {
     rec = record(i);
     // Delete record if the zone is assign to the container
     if (rec.value("Container").toInt()==containerId)
         removeRow(i);
    }
    submitAll();
}
