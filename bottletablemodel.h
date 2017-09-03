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

//    void repositionBottle(int id);TODELETE

public slots:
    void changeRectangleData(QRectF data, int id);
    void changeContainerRectangleData(QRectF data, int id);
    void changeContainer(int bottleId, int newContainerId);
    void changeZone(int bottleId, int newZoneId);
    void deleteBottle(int bottleId);

};

#endif // BOTTLETABLEMODEL_H
