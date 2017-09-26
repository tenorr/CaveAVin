#ifndef STORAGEDELEGATE_H
#define STORAGEDELEGATE_H

#include "cellar.h"
class AbstractStorageScene;
#include "rackelement.h"
#include "matrixrackelement.h"
#include "storagebottle.h"
#include "zone.h"

#include <QObject>
#include <QSqlDatabase>

class Cellar;
class StorageBottle;

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
    StorageBottle *createBottle(QPointF position, Zone *zone = Q_NULLPTR, int radius = 40, int rackElementId=0);

    QList<Zone *> zones() const;
    void setZones();
    
    QList<StorageBottle *> bottles();
    void setBottles();
    void appendBottle(StorageBottle * bottle);

    QList<RackElement *> rackElements();
    void setRackElements();
    RackElement * rackElementItem(int number);

    void setRackElementEnabled(RackElement *item, bool fEnabled);

    void setRackCircular(bool fCircular);
    
    StorageBottle *findBottle(int bottleId);

    int storageId() const;
    void setStorageId(int storageId);

    bool connectCellar() const;
    void setConnectCellar(bool connectCellar=true);

    QPointF ratios() const;
    void setRatios(const QPointF &ratios);

    QPointF convertPosition(QPointF storagePosition);

    int storageRowCount() const;
    void setStorageRowCount(int storageRowCount);

    int storageColumnCount() const;
    void setStorageColumnCount(int storageColumnCount);

    int rackHint() const;
    void setRackHint(int rackHint);

    Zone *findBottleZone(int zoneId);
    void connectZoneSignals(Zone* zone);
    void connectStorageBottleSignals(StorageBottle *bottle);
    QPointF toZonePosition(QPointF position, Zone* zone);
    QString disableItemsText();
    
public slots:
    void deleteZone(int zoneId);
    void deleteBottle(int bottleId);
    void positionBottleOnCellar(int bottleId, QPointF position);

private:
    int m_storageId;
    QList<Zone *> m_zones;
    QList<StorageBottle *> m_bottles;
    QList<RackElement *> m_rackElements;
    QPointF m_ratios;
    int m_storageRowCount;
    int m_storageColumnCount;
    int m_rackHint;
    bool m_connectCellar;
};

#endif // STORAGEDELEGATE_H
