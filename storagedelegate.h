#ifndef STORAGEDELEGATE_H
#define STORAGEDELEGATE_H

#include "cellar.h"
class AbstractStorageScene;
#include "storagebottle.h"
#include "zone.h"

#include <QObject>
#include <QSqlDatabase>

class Cellar;

class StorageDelegate : public QObject
{
    Q_OBJECT
public:
    StorageDelegate(int storageId, QObject *parent=Q_NULLPTR);

    Cellar *cellar();
    int cellarId();
    QSqlDatabase db();
    BottleTableModel *bottleModel();
    ZoneTableModel *zoneModel();

    Zone *createZone(QPointF position);
    StorageBottle *createBottle(QPointF position, Zone *zone = Q_NULLPTR);

    QList<Zone *> zones() const;
    void setZones();
    
    QList<StorageBottle *> bottles() const;
    void setBottles();
    
    int storageId() const;
    void setStorageId(int storageId);

    bool connectCellar() const;
    void setConnectCellar(bool connectCellar=true);

    QPointF ratios() const;
    void setRatios(const QPointF &ratios);

    QPointF convertPosition(QPointF storagePosition);

private:
    Zone *findBottleZone(int zoneId);
    void connectZoneSignals(Zone* zone);
    void connectStorageBottleSignals(StorageBottle* bottle);
    QPointF toZonePosition(QPointF position, Zone* zone);
    
public slots:
    void deleteZone(int zoneId);
    void deleteBottle(int bottleId);
    void positionBottleOnCellar(int bottleId, QPointF position);

private:
    int m_storageId;
    QList<Zone *> m_zones;
    QList<StorageBottle *> m_bottles;
    QPointF m_ratios;
    bool m_connectCellar;
};

#endif // STORAGEDELEGATE_H
