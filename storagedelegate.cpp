#include "storagedelegate.h"
#include <QDebug>

StorageDelegate::StorageDelegate(int storageId, QObject *parent)
    :QObject (parent)
{
    setConnectCellar(false); // connection not activated by default
    setStorageId(storageId);

 // Find every zone, bottles and rack >elements of the storage
    setZones();
    setBottles();
    setRackElements();
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

StorageBottle *StorageDelegate::createBottle(QPointF position, Zone *zone, int radius, int rackElementId)
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
     rec.setValue("StorageR",radius);
     rec.setValue("RackElement",rackElementId);

     // Create temporary bottle
     StorageBottle *bottle = new StorageBottle(rec,bottleModel(),zone);

     // Create a new SQL record if dialog accepted and return bottle
     if (bottle->changeBottleData(rec,-1)) {
        bottleModel()->submitAll();
        bottleModel()->select();
        connectStorageBottleSignals(bottle);
        appendBottle(bottle);
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


QList<RackElement *> StorageDelegate::rackElements()
{
    return m_rackElements;
}

void StorageDelegate::setRackElements()
{
    QList <RackElement *> rackElements;
    QVector<int> disabledItems; // List of disabledElement

   // Find data for RackElements
    QString storageData = QString();
    int rowCount =0;
    int columnCount =0;

    QSqlQuery query;
    query.prepare(QString("SELECT StorageData, RowCount, ColumnCount, DisabledItems, RackHint FROM Storage WHERE Id = %1").arg(storageId()));
    query.exec();
    if (query.first()) {
        storageData = query.value("StorageData").toString();
        rowCount = query.value("RowCount").toInt();
        columnCount = query.value("ColumnCount").toInt();
        setRackHint(query.value("RackHint").toInt());
        disabledItems = StorageTableModel::disableItems(query.value("DisabledItems").toString());
    }

// Build RackElements
    for (int i =0;i<rowCount;i++) {
        for (int j=0;j<columnCount;j++)  {
            // Find if Rectangle or Circle text
            if ((storageData == "Rectangle") || (storageData == "Circle")) {
                int number = i*columnCount+j+1;
                MatrixRackElement *item= new MatrixRackElement(number,(storageData == "Circle"),(disabledItems.indexOf(number) == -1));
                item->setRect(QRectF(0,0,rackHint(),rackHint()));
                rackElements << item;
            }
       }
    }

    m_rackElements = rackElements;
}

RackElement *StorageDelegate::rackElementItem(int number)
{
    // Return the item by number
    foreach (RackElement *item, rackElements()) {
        if (item->number() == number)
            return item;
    }
    return Q_NULLPTR;   // If not found
}

void StorageDelegate::setRackElementEnabled(RackElement *item, bool fEnabled)
{
    // Change enabled flag in item
    item->enableElement(fEnabled);

    // Change the disabled list in the database
    QSqlQuery query;
    query.prepare(QString("UPDATE Storage SET DisabledItems = '%1' WHERE Id = %2").arg(disableItemsText()).arg(storageId()));
    query.exec();
}

void StorageDelegate::setRackCircular(bool fCircular)
{
    foreach (RackElement * rackElement, rackElements()) {
        MatrixRackElement * item = static_cast<MatrixRackElement *>(rackElement);
        if (item)
            item->setCircular(fCircular);
    }

    // Update DataBase
    QString storageData = (fCircular)? "Circle" : "Rectangle";
    QSqlQuery query;
    query.prepare(QString("UPDATE Storage SET StorageData = '%1' WHERE Id = %2").arg(storageData).arg(storageId()));
    query.exec();
}


int StorageDelegate::rackHint() const
{
    return m_rackHint;
}

void StorageDelegate::setRackHint(int rankHint)
{
    m_rackHint = rankHint;
}

int StorageDelegate::storageColumnCount() const
{
    return m_storageColumnCount;
}

void StorageDelegate::setStorageColumnCount(int storageColumnCount)
{
    m_storageColumnCount = storageColumnCount;
}

int StorageDelegate::storageRowCount() const
{
    return m_storageRowCount;
}

void StorageDelegate::setStorageRowCount(int storageRowCount)
{
    m_storageRowCount = storageRowCount;
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

QString StorageDelegate::disableItemsText()
{
    // Retrieve Disabled List
      QVector<int> disabledItems; // List of disabledElement
      foreach (RackElement * item, rackElements()) {
          if (!item->isEnabled())
              disabledItems << item->number();
      }
    // Sort list
    std::sort(disabledItems.begin(),disabledItems.end());

    // Build text
    QString str = QString();
    if (!disabledItems.isEmpty()) {
        str ="{";
        foreach (int n, disabledItems) {
            str += QString::number(n)+",";
        }
        str.chop(1); // Delete last coma
        str +="}";
    }
    return str;
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
        connect(bottle,&StorageBottle::wineTypeChanged,cellar(),&Cellar::changeBottleWineType);
        if (connectCellar())
            connect(bottle, &StorageBottle::bottlePositioningRequested,this,&StorageDelegate::positionBottleOnCellar);
    }
}

QList<StorageBottle *> StorageDelegate::bottles()
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

void StorageDelegate::appendBottle(StorageBottle *bottle)
{
    m_bottles.append(bottle);
}

StorageBottle *StorageDelegate::findBottle(int bottleId)
{
    foreach (StorageBottle *bottle, bottles()) {
          if (bottle->id() == bottleId)
                return bottle;
    }
    return Q_NULLPTR;
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
