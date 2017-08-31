#include "room.h"
#include <QDebug>
#include <QFile>

Room::Room(ContainerTableModel *containerModel, BottleTableModel *bottleModel, ZoneTableModel *zoneModel, int id, QObject *parent)
    :AbstractScene(id, parent)
{
    // Set db,and table model pointers
    setDb(containerModel->database());
    setContainerModel(containerModel);
    setBottleModel(bottleModel);
    setZoneModel(zoneModel);
    setTableName("Room");

 // Set Room Id & Name
    setName(new GraphicsText);

 // Add rectangle and name into scene
    QPen pen;
    pen.setWidth(10);
    addRect(10,10,1960,1200,pen);

    // Create Context Menu
    createContextMenu();

 // Populate Room with containers
    for(int i=0;i<containerModel->rowCount();i++) {
     addContainer(containerModel->record(i));
    }

// Populate Room with bottles
    for(int i=0;i<bottleModel->rowCount();i++) {
    addBottle(bottleModel->record(i));
    }
    // Connect bottle Table Model for repositioning
    connect(bottleModel,&BottleTableModel::bottleReposioned,this,&Room::repositionBottle);
}

Room::~Room()
{

}

void Room::createContainer()
{
    // Create a new record into the tableModel
  QSqlRecord rec = containerModel()->createNew(id(),contextPosition());

    // Create new Container
    addContainer(rec);
}

void Room::deleteContainer(int id)
{
    // Delete record in table Model and selected Item

    if (containerModel()->deleteRecordId(id))
        delete selectedItems()[0];
}

void Room::createBottle()
{
    // Retrieve SQL record
    QSqlRecord rec = bottleModel()->record();
    int newId = bottleModel()->newId();
    rec.setValue("Id",newId);
    rec.setValue("PurchaseDate",QDate::currentDate());
    rec.setValue("Room", id());
    rec.setValue("RoomX",contextPosition().x());
    rec.setValue("RoomY",contextPosition().y());
    rec.setValue("RoomR",40);
    rec.setValue("ContainerR",40);

    Bottle *bottle = new Bottle(rec, this);

    // Create a new SQL record if dialog accepted and add bottle
    if (bottle->changeBottleData(rec,-1)) {
        addBottle(rec);
        bottleModel()->select();
    }
    delete bottle;
}

void Room::repositionBottle(int bottleId, QPoint pos)
{
    bool fFound = false;
    // Find the bottle in the scene
    QList<QGraphicsItem *> itemList = items();
    foreach (QGraphicsItem * item, itemList) {
        if (item->type()== QGraphicsItem::UserType +1) {
            Bottle *bottle = static_cast<Bottle *>(item);
            if (bottle->id() == bottleId) {
                bottle->setPos(pos);
                fFound = true;
                break;
            }
        }
    }
    if (!fFound) {
        // Retrieve the record of the bottle
           int row = bottleModel()->rowPosition(bottleId);
           if (row !=-1) {
               // Add a new bottle if exists
               QSqlRecord rec = bottleModel()->record(row);
               addBottle(rec);
        }
    }
}

void Room::addContainer(QSqlRecord rec)
{
    // Check if rec is about to the room
    if (rec.value("Room").toInt()==id()) {
    // Create and add new container
        Container * container = new Container(rec,this);
        addItem(container);

    // Connect signals
        connect(container,&Container::colorChanged,containerModel(),&ContainerTableModel::changeColor);
        connect(container,&Container::brushStyleChanged,containerModel(),&ContainerTableModel::changeBrushStyle);
        connect(container,&Container::rectangleDataChanged,containerModel(),&ContainerTableModel::changeRectangleData);
        connect(container,&Container::itemToBeDeleted,this,&Room::deleteContainer);
    }
}

void Room::addBottle(QSqlRecord rec)
{
    // Check if rec is about to the room
    if (rec.value("Room").toInt()==id()) {
    // Create and add new bottle
        Bottle * bottle = new Bottle(rec,this);
     // Don't add again a bottle in the container
        if (bottle->sublocationId()==0)
            addItem(bottle);

    // Connect signals
        connect(bottle,&Bottle::rectangleDataChanged,bottleModel(),&BottleTableModel::changeRectangleData);
        connect(bottle,&Bottle::bottleToBeDeleted,this,&Room::deleteBottle);
    }
}

void Room::createContextMenu()
{
    QMenu * menu = new QMenu;
    menu->addAction(tr("Create Container"),this,SLOT(createContainer()));
    menu->addAction(tr("Create Bottle"),this,SLOT(createBottle()));
    setContextMenu(menu);
}

ContainerTableModel *Room::containerModel() const
{
    return m_containerModel;
}

void Room::setContainerModel(ContainerTableModel *containerModel)
{
    m_containerModel = containerModel;
}



