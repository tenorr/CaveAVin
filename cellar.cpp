#include "cellar.h"
#include <QDebug>
#include <QFile>

Cellar::Cellar(StorageTableModel *storageModel, BottleTableModel *bottleModel, ZoneTableModel *zoneModel, int id, QObject *parent)
    :AbstractScene(id, parent)
{
    // Set db,and table model pointers
    setDb(storageModel->database());
    setStorageModel(storageModel);
    setBottleModel(bottleModel);
    setZoneModel(zoneModel);
    setTableName("Cellar");

 // Set Cellar Id & Name
    setName(new GraphicsText);

 // Add rectangle and name into scene

    // Retrieve parent geometry and set scene rectangle
    QSize sceneRectSize = static_cast<QWidget *>(parent)->size();
    setSceneRect(QRectF(QPointF(0,0),sceneRectSize));

    // Draw a rectangle to border it
    QSizeF rectangleSize = QSizeF(sceneRectSize) - QSizeF(20,25);

    QPen pen;
    pen.setWidth(10);
    addRect(QRectF(QPointF(10,10),rectangleSize),pen);

    // Create Context Menu
    createContextMenu();

 // Populate Cellar with storages
    for(int i=0;i<storageModel->rowCount();i++) {
     addStorage(storageModel->record(i));
    }

// Populate Cellar with bottles
    for(int i=0;i<bottleModel->rowCount();i++) {
    addBottle(bottleModel->record(i));
    }
}

Cellar::~Cellar()
{

}

void Cellar::createStorage()
{
    // Create a new record into the tableModel
  QSqlRecord rec = StorageModel()->createNew(id(),contextPosition());

    // Create new Storage
    addStorage(rec);
}

void Cellar::deleteStorage(int id)
{
    // Delete record in table Model and selected Item
    if (StorageModel()->deleteRecordId(id))
        delete selectedItems()[0];
}

void Cellar::createBottle()
{
    // Retrieve SQL record
    QSqlRecord rec = bottleModel()->record();
    int newId = bottleModel()->newId();
    rec.setValue("Id",newId);
    rec.setValue("PurchaseDate",QDate::currentDate());
    rec.setValue("Cellar", id());
    rec.setValue("CellarX",contextPosition().x());
    rec.setValue("CellarY",contextPosition().y());
    rec.setValue("CellarR",40);
    rec.setValue("StorageR",40);

    Bottle *bottle = new Bottle(rec, this);

    // Create a new SQL record if dialog accepted and add bottle
    if (bottle->changeBottleData(rec,-1)) {
        addBottle(rec);
        bottleModel()->select();
    }
    delete bottle;
}

void Cellar::changeBottleWineType(int bottleId, int wineTypeId)
{
    Bottle * bottle = findBottle(bottleId);
    if (bottle)
        bottle->setWineType(wineTypeId);
}

void Cellar::addStorage(QSqlRecord rec)
{
    // Check if rec is about to the Cellar
    if (rec.value("Cellar").toInt()==id()) {
    // Create and add new Storage
        Storage * storage = new Storage(rec,this);
        addItem(storage);

    // Connect signals
        connect(storage,&Storage::colorChanged,StorageModel(),&StorageTableModel::changeColor);
        connect(storage,&Storage::brushStyleChanged,StorageModel(),&StorageTableModel::changeBrushStyle);
        connect(storage,&Storage::rectangleDataChanged,StorageModel(),&StorageTableModel::changeRectangleData);
        connect(storage,&Storage::itemToBeDeleted,this,&Cellar::deleteStorage);
    }
}

void Cellar::addBottle(QSqlRecord rec)
{
    // Check if rec is about to the Cellar
    if (rec.value("Cellar").toInt()==id()) {
    // Create and add new bottle
        Bottle * bottle = new Bottle(rec,this);
     // Don't add again a bottle in the Storage
        if (bottle->sublocationId()==0)
            addItem(bottle);

    // Connect signals
       connect(bottle,&Bottle::bottleToBeDeleted,this,&Cellar::deleteBottle);
       connect(bottle,&Bottle::rectangleDataChanged,bottleModel(),&BottleTableModel::changeRectangleData);
    }
}

void Cellar::createContextMenu()
{
    QMenu * menu = new QMenu;
    menu->addAction(tr("Create Storage"),this,SLOT(createStorage()));
    menu->addAction(tr("Create Bottle"),this,SLOT(createBottle()));
    setContextMenu(menu);
}

StorageTableModel *Cellar::StorageModel() const
{
    return m_storageModel;
}

void Cellar::setStorageModel(StorageTableModel *storageModel)
{
    m_storageModel = storageModel;
}

Bottle *Cellar::findBottle(int bottleId)
{
    // Find the bottle in the scene
    QList<QGraphicsItem *> itemList = items();
     foreach (QGraphicsItem * item, itemList) {
        if (item->type()== QGraphicsItem::UserType +1) {
            Bottle *bottle = static_cast<Bottle *>(item);
            if (bottle->id() == bottleId) {
                return bottle;
                break;
            }
        }
    }
     return Q_NULLPTR;
}

Bottle * Cellar::createBottleFromTableData(int bottleId)
{
    // Retrieve the record of the bottle
    int row = bottleModel()->rowPosition(bottleId);

    if (row !=-1) {
        // Add a new bottle if exists
        QSqlRecord rec = bottleModel()->record(row);
        addBottle(rec);
    }
    return findBottle(bottleId); // return true if add Bottle is achieved
}

void Cellar::positionBottle(int bottleId, QPointF position)
{
    Bottle * bottle = findBottle(bottleId);
    // Create Bottle if not found according to the tableModel data
    if (!bottle)
        bottle = createBottleFromTableData(bottleId);

    // Position the bottle in the Storage
    if (bottle)
        bottle->setPos(position);
}

void Cellar::clearBottle(int bottleId)
{
    // Delete bottle item from scene (without deleting bottle record)
    findBottle(bottleId)->deleteLater();
}

