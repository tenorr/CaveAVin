#ifndef BOTTLETABLEMODEL_H
#define BOTTLETABLEMODEL_H

#include "abstractwinetablemodel.h"
#include <QRect>
#include <QSqlQuery>

class BottleTableModel : public AbstractWineTableModel
{
    Q_OBJECT
public:
    BottleTableModel(QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());

public slots:
    void changeRectangleData(QRect data, int id);
    void changeContainerRectangleData(QRect data, int id);
    void changeContainer(int bottleId, int newContainerId);
    void changeZone(int bottleId, int newZoneId);
    void deleteBottle(int bottleId);

signals:
    void bottleReposioned(int id, QPoint pos);

};

#endif // BOTTLETABLEMODEL_H
