#include "bottletablemodel.h"
#include <QDebug>


BottleTableModel::BottleTableModel(QObject *parent, QSqlDatabase db)
    : AbstractWineTableModel("Bottle",parent,db)
{

}


void BottleTableModel::changeRectangleData(QRect data, int id)
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

void BottleTableModel::changeContainerRectangleData(QRect data, int id)
{
    // Change Rectangle Data
    int row = rowPosition(id);

    if (row !=-1) {
    QSqlRecord rec = record(row);
    rec.setValue("ContainerX",data.x());
    rec.setValue("ContainerY",data.y());
    rec.setValue("ContainerR",data.width());

    // Retrieve Container Ratios
    // Prepare Query to retrieve width and height
    QSqlQuery query;
    query.prepare("SELECT XRatio, YRatio FROM Container WHERE Id = :id");
    query.bindValue(":id",rec.value("Container").toInt());
    query.exec();
    query.next();
    qreal xRatio = query.value(0).toDouble();
    qreal yRatio = query.value(1).toDouble();

    // Store new coordinates in Room
    rec.setValue("RoomX",int(double(data.x())/xRatio));
    rec.setValue("RoomY",int(double(data.y())/yRatio));

    setRecord(row,rec);
    submitAll();
    // Emit signal for repositioning the bootle in the Room
    QPoint pos = QPoint(int(double(data.x())/xRatio),int(double(data.y())/yRatio));
    emit bottleReposioned(id, pos);
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
