#include "bottletablemodel.h"
#include <QDebug>


BottleTableModel::BottleTableModel(QObject *parent, QSqlDatabase db)
    : AbstractWineTableModel("Bottle",parent,db)
{

}

QPointF BottleTableModel::storagePosition(int id)
{
    QPointF position = QPointF();
    int row = rowPosition(id);
     if (row !=-1) {
         QSqlRecord rec = record(row);
         position = QPointF(rec.value("StorageX").toDouble(),rec.value("StorageY").toDouble());
     }
     return position;
}

void BottleTableModel::setStoragePosition(int id, QPointF position)
{
     int row = rowPosition(id);
     if (row !=-1) {
         QSqlRecord rec = record(row);
         rec.setValue("StorageX",position.x());
         rec.setValue("StorageY",position.y());
         setRecord(row,rec);
         submitAll();
     }
}


void BottleTableModel::changeRectangleData(QRectF data, int id)
{
    // Change Rectangle Data
    int row = rowPosition(id);

    if (row !=-1) {
    QSqlRecord rec = record(row);
    rec.setValue("CellarX",data.x());
    rec.setValue("CellarY",data.y());
    rec.setValue("CellarR",data.width());
    setRecord(row,rec);
    submitAll();
    }
}

void BottleTableModel::changeStorageRectangleData(QRectF data, int id)
{
    // Change Rectangle Data
    int row = rowPosition(id);

    if (row !=-1) {
        QSqlRecord rec = record(row);
        rec.setValue("StorageX",data.x());
        rec.setValue("StorageY",data.y());
        rec.setValue("StorageR",data.width());
        setRecord(row,rec);
        submitAll();
    }

}

void BottleTableModel::changeStorage(int bottleId, int newStorageId)
{
    int row = rowPosition(bottleId);

    if (row !=-1) {
    QSqlRecord rec = record(row);
    rec.setValue("Storage",newStorageId);
    rec.setValue("RackElement",0);
    rec.setValue("Zone",0);
    rec.setValue("StorageX",0);
    rec.setValue("StorageY",0);
    setRecord(row,rec);
    submitAll();
    }
}

void BottleTableModel::changeRackElement(int bottleId, int newRackElementId)
{
    int row = rowPosition(bottleId);

    if (row !=-1) {
    QSqlRecord rec = record(row);
    rec.setValue("RackElement",newRackElementId);
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
    rec.setValue("RackElement",0);
    setRecord(row,rec);
    submitAll();
    }
}

bool BottleTableModel::deleteBottle(int bottleId)
{
    int row = rowPosition(bottleId);
    if (row !=-1) {
        removeRow(row);
        submitAll();
    }
    return (row !=-1);
}

