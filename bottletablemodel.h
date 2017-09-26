#ifndef BOTTLETABLEMODEL_H
#define BOTTLETABLEMODEL_H

#include "abstractwinetablemodel.h"
#include <QRectF>
#include <QSqlQuery>

class BottleTableModel : public AbstractWineTableModel
{
    Q_OBJECT
public:
    BottleTableModel(QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());

    QPointF storagePosition(int id);
    void setStoragePosition(int id, QPointF position);

    //    void repositionBottle(int id);TODELETE

public slots:
    void changeRectangleData(QRectF data, int id);
    void changeStorageRectangleData(QRectF data, int id);
    void changeStorage(int bottleId, int newStorageId);
    void changeRackElement (int bottleId, int newRackElementId);
    void changeZone(int bottleId, int newZoneId);
    bool deleteBottle(int bottleId);

};

#endif // BOTTLETABLEMODEL_H
