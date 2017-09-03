#include "bottletablemodel.h"
#include <QDebug>


BottleTableModel::BottleTableModel(QObject *parent, QSqlDatabase db)
    : AbstractWineTableModel("Bottle",parent,db)
{

}


void BottleTableModel::changeRectangleData(QRectF data, int id)
{
    // Change Rectangle Data
    int row = rowPosition(id);

    if (row !=-1) {
    QSqlRecord rec = record(row);
    rec.setValue("RoomX",data.x());
    rec.setValue("RoomY",data.y());
    rec.setValue("RoomR",data.width());
    setRecord(row,rec);
    submitAll();
    }
}

void BottleTableModel::changeContainerRectangleData(QRectF data, int id)
{
    // Change Rectangle Data
    int row = rowPosition(id);

    if (row !=-1) {
        QSqlRecord rec = record(row);
        rec.setValue("ContainerX",data.x());
        rec.setValue("ContainerY",data.y());
        rec.setValue("ContainerR",data.width());
        setRecord(row,rec);
        submitAll();
    }

}

void BottleTableModel::changeContainer(int bottleId, int newContainerId)
{
    int row = rowPosition(bottleId);

    if (row !=-1) {
    QSqlRecord rec = record(row);
    rec.setValue("Container",newContainerId);
    rec.setValue("Zone",0);
    rec.setValue("ContainerX",0);
    rec.setValue("ContainerY",0);
    setRecord(row,rec);
    submitAll();
    }
}

void BottleTableModel::changeZone(int bottleId, int newZoneId)
{
    int row = rowPosition(bottleId);

    if (row !=-1) {
    QSqlRecord rec = record(row);
    rec.setValue("Zone",newZoneId);
    setRecord(row,rec);
    submitAll();
    }
}

void BottleTableModel::deleteBottle(int bottleId)
{
    int row = rowPosition(bottleId);
    if (row !=-1) {
    removeRow(row);
    submitAll();
    }
}

