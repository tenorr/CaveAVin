#include "storagedelegate.h"

StorageDelegate::StorageDelegate(int storageId, QObject *parent)
    :QObject (parent)
{
    setConnectCellar(false); // connection not activated by default
    setStorageId(storageId);
 // Find every zone and bottles of the storage
    setZones();
    setBottles();
}

Cellar *StorageDelegate::cellar()
{
    return static_cast<Cellar *>(parent());
}

int StorageDelegate::cellarId()
{
    return cellar()->id();
}

QSqlDatabase StorageDelegate::db()
{
    return cellar()->db();
}

BottleTableModel *StorageDelegate::bottleModel()
{
    return cellar()->bottleModel();
}

ZoneTableModel *StorageDelegate::zoneModel()
{
    return cellar()->zoneModel();
}

Zone *StorageDelegate::createZone(QPointF position)
{
    // Create a new record into the tableModel
   QSqlRecord rec = zoneModel()->createNew(storageId(),position);

   // Create new Zone
   Zone * zone = new Zone(rec);

   // Connect signals
   connectZoneSignals(zone);
   zones() << zone;
   return zone;
}

StorageBottle *StorageDelegate::createBottle(QPointF position, Zone *zone)
{
    // Retrieve SQL record and complete position information
     QSqlRecord rec = bottleModel()->record();
     int newId = bottleModel()->newId();
     rec.setValue("Id",newId);
     rec.setValue("PurchaseDate",QDate::currentDate());
     rec.setValue("Cellar", cellarId());
     rec.setValue("Storage", storageId());
     if (zone)
         rec.setValue("Zone", zone->id());
     rec.setValue("StorageX", toZonePosition(position,zone).x());
     rec.setValue("StorageY", toZonePosition(position,zone).y());
     rec.setValue("CellarR",40);
     rec.setValue("StorageR",40);

     // Create temporary bottle
     StorageBottle *bottle = new StorageBottle(rec,bottleModel(),zone);

     // Create a new SQL record if dialog accepted and return bottle
     if (bottle->changeBottleData(rec,-1)) {
        bottleModel()->submitAll();
        bottleModel()->select();
        connectStorageBottleSignals(bottle);
        bottles() << bottle;
        return bottle;
            }
    // if not, delete newly created Storage Bottle and return Null
    bottle->deleteLater();
    return Q_NULLPTR;
}

void StorageDelegate::deleteZone(int zoneId)
{
    Zone* zoneSender = qobject_cast <Zone *> (sender());

    if (zoneSender) {
    // Delete record in table Model and sender Item
        if (zoneModel()->deleteRecordId(zoneId))
        // Delete Zone
            zoneSender->deleteLater();
    }
}

void StorageDelegate::deleteBottle(int bottleId)
{
    StorageBottle *bottleSender = qobject_cast <StorageBottle *> (sender());

    if(bottleSender) {
        // Delete record in table Model and sender Item
            if (bottleModel()->deleteBottle(bottleId))
             // Delete relevant bottle in cellar
                cellar()->clearBottle(bottleId);
             // Delete Bottle
                bottleSender->deleteLater();
            // D
    }
}

void StorageDelegate::positionBottleOnCellar(int bottleId, QPointF position)
{
    // Apply ratios to storage position and update cellar
    cellar()->positionBottle(bottleId,convertPosition(position));
}

QPointF StorageDelegate::ratios() const
{
    return m_ratios;
}

void StorageDelegate::setRatios(const QPointF &ratios)
{
    m_ratios = ratios;
}

QPointF StorageDelegate::convertPosition(QPointF storagePosition)
{
    // Convert position from Storage to Cellar thanks to ratios
    return QPointF(storagePosition.x()/ratios().x(), storagePosition.y()/ratios().y());
}

QPointF StorageDelegate::toZonePosition(QPointF position, Zone *zone)
{
    if (zone)
        return position - zone->scenePos();

    return position;
}

bool StorageDelegate::connectCellar() const
{
    return m_connectCellar;
}

void StorageDelegate::setConnectCellar(bool connectCellar)
{
    foreach (StorageBottle *bottle, bottles()) {
        if (connectCellar)
            connect(bottle, &StorageBottle::bottlePositioningRequested,this,&StorageDelegate::positionBottleOnCellar);
        else
            disconnect(bottle, &StorageBottle::bottlePositioningRequested,this,&StorageDelegate::positionBottleOnCellar);
    }

    foreach (Zone *zone, zones()) {
        if (connectCellar)
            connect(zone, &Zone::bottlePositioningRequested,this,&StorageDelegate::positionBottleOnCellar);
        else
            disconnect(zone, &Zone::bottlePositioningRequested,this,&StorageDelegate::positionBottleOnCellar);
    }

    m_connectCellar = connectCellar;
}

int StorageDelegate::storageId() const
{
    return m_storageId;
}

void StorageDelegate::setStorageId(int storageId)
{
    m_storageId = storageId;
}

Zone *StorageDelegate::findBottleZone(int zoneId)
{
    if(zoneId >0) {
        foreach (Zone * zone, zones()) {
            if (zone->id() == zoneId)
                return zone;
        }
    }
    return Q_NULLPTR;
}

void StorageDelegate::connectZoneSignals(Zone *zone)
{
    if (zone) {
        connect(zone,&Zone::colorChanged,zoneModel(),&ZoneTableModel::changeColor);
        connect(zone,&Zone::brushStyleChanged,zoneModel(),&ZoneTableModel::changeBrushStyle);
        connect(zone,&Zone::rectangleDataChanged,zoneModel(),&ZoneTableModel::changeRectangleData);
        connect(zone,&Zone::itemToBeDeleted,this,&StorageDelegate::deleteZone);
        if (connectCellar())
            connect(zone, &Zone::bottlePositioningRequested,this,&StorageDelegate::positionBottleOnCellar);
    }
}

void StorageDelegate::connectStorageBottleSignals(StorageBottle *bottle)
{
    if (bottle) {
        connect(bottle,&StorageBottle::rectangleDataChanged,bottleModel(),&BottleTableModel::changeStorageRectangleData);
        connect(bottle,&StorageBottle::bottleToBeDeleted,this,&StorageDelegate::deleteBottle);
        if (connectCellar())
            connect(bottle, &StorageBottle::bottlePositioningRequested,this,&StorageDelegate::positionBottleOnCellar);
    }
}

QList<StorageBottle *> StorageDelegate::bottles() const
{
    return m_bottles;
}

void StorageDelegate::setBottles()
{
    QList<StorageBottle *> bottles;
    for (int i=0, n = bottleModel()->rowCount();i<n;i++) {
        QSqlRecord rec = bottleModel()->record(i);
         if (rec.value("Storage").toInt() == storageId()) {
                StorageBottle *bottle = new StorageBottle(rec,bottleModel(),findBottleZone(rec.value("Zone").toInt()));
                connectStorageBottleSignals(bottle);
                bottles << bottle;
             }
         }
    m_bottles = bottles;
}

QList<Zone *> StorageDelegate::zones() const
{
    return m_zones;
}

void StorageDelegate::setZones()
{
    QList<Zone *> zones;
    for (int i=0, n=zoneModel()->rowCount();i<n;i++) {
        QSqlRecord rec = zoneModel()->record(i);
        if (rec.value("Storage").toInt() == storageId()) {
            Zone * zone = new Zone(rec);
            connectZoneSignals(zone);
            zones << zone;
         }
    }
    m_zones = zones;
}
