#include "containerscene.h"
#include <QDebug>

ContainerScene::ContainerScene(int containerId, Room &room, QObject *parent)
    : AbstractScene(containerId, parent)
{
    // Set SQL Tables
    setDb(room.db());
    setBottleModel(room.bottleModel());
    setZoneModel(room.zoneModel());
    setTableName("Container");

    // Add color and brush style
       QPen pen;
       pen.setWidth(10);
       pen.setColor(color());
       QBrush brush(color());
       brush.setStyle(brushStyle());

       addRect(10,10,1960,1200,pen,brush);
       setName(new GraphicsText);

   // Create Context Menu
       createContextMenu();

  // Store container dimensions
       setRatio();

   // Populate Container with zones
          for(int i=0;i<zoneModel()->rowCount();i++) {
           addZone(zoneModel()->record(i));
          }
    // Populate Container with bottles
           for(int i=0;i<bottleModel()->rowCount();i++) {
            addBottle(bottleModel()->record(i));
              }
}

QColor ContainerScene::color()
{
    // Find Name in dataBase
    QSqlQuery query;
    query.prepare("SELECT Red, Green, Blue FROM Container WHERE Id = :id");
    query.bindValue(":id",id());
    query.exec();
    query.next();

    return QColor(query.value(0).toInt(),query.value(1).toInt(),query.value(2).toInt());
}

Qt::BrushStyle ContainerScene::brushStyle()
{
    // Find Name in dataBase
    QSqlQuery query;
    query.prepare("SELECT BrushStyle FROM Container WHERE Id = :id");
    query.bindValue(":id",id());
    query.exec();
    query.next();

    return (Qt::BrushStyle) query.value(0).toInt();
}

void ContainerScene::createContextMenu()
{
    QMenu * menu = new QMenu;
    menu->addAction(tr("Create Zone"), this,SLOT(createZone()));
    menu->addAction(tr("Create Bottle"));
    setContextMenu(menu);
}

void ContainerScene::createZone()
{
    // Create a new record into the tableModel
   QSqlRecord rec = zoneModel()->createNew(id(),contextPosition());

    // Create new Zone
   addZone(rec);
}

void ContainerScene::deleteZone(int zoneId)
{
    // Delete record in table Model and selected Item

    if (zoneModel()->deleteRecordId(zoneId))
        delete selectedItems()[0];
}

void ContainerScene::createBottle()
{
    // Retrieve SQL record
    QSqlRecord rec = bottleModel()->record();
    int newId = bottleModel()->newId();
    rec.setValue("Id",newId);
    BottleDialog* dialog = new BottleDialog(db(),rec);
    if (dialog->exec()==QDialog::Accepted) {
        rec = bottleModel()->record();
        rec.setValue("Id",newId);
        rec.setValue("Wine",dialog->wineId());
        int index = dialog->index("Millesime");
        int millesime = dialog->combo().at(index)->currentText().toInt();
        rec.setValue("Millesime",millesime);
        rec.setValue("Container", id());
        rec.setValue("ContainerX",contextPosition().x());
        rec.setValue("ContainerY",contextPosition().y());
        rec.setValue("RoomR",40);
        rec.setValue("ContainerR",40);

     // Retrieve information about container
        QSqlQuery query;
        query.prepare("SELECT Room FROM Container WHERE Id = :id");
        query.bindValue(":id",id());
        query.exec();
        query.next();
        rec.setValue("Room",query.value(0));
        /*
        QGraphicsItem *item = itemAt(contextPosition(),QTransform());
        if (item !=0) {
            if (item->type()=QGraphicsItem::UserType+2) {
                    int zoneId = static_cast<Zone *>(item)->id();
                    rec.setValue("Zone",zoneId);
            }
        }
        */
        bottleModel()->insertRecord(-1,rec);
        bottleModel()->submitAll();
    }
    delete dialog;
    addBottle(rec);
}


void ContainerScene::setRatio()
{
    // Prepare Query to retrieve width and height
    QSqlQuery query;
    query.prepare("SELECT Width, Height FROM Container WHERE Id = :id");
    query.bindValue(":id",id());
    query.exec();
    query.next();
    qreal xRatio = width()/query.value(0).toInt();
    qreal yRatio = height()/query.value(1).toInt();

    // Store ratios in SQL Table
    query.prepare(QString("UPDATE Container SET XRatio = :XRatio , YRatio = :YRatio WHERE Id = :id"));
    query.bindValue(":id",id());
    query.bindValue(":XRatio",xRatio);
    query.bindValue(":YRatio",yRatio);
    query.exec();
}

void ContainerScene::addZone(QSqlRecord rec)
{
    // Check if record is about the container
    if (rec.value("Container").toInt()==id()) {
        // Create and add new Zone
        Zone * zone = new Zone(rec);
        addItem(zone);

    // Connect signals
        connect(zone,&Zone::colorChanged,zoneModel(),&ZoneTableModel::changeColor);
        connect(zone,&Zone::brushStyleChanged,zoneModel(),&ZoneTableModel::changeBrushStyle);
        connect(zone,&Zone::rectangleDataChanged,zoneModel(),&ZoneTableModel::changeRectangleData);
        connect(zone,&Zone::itemToBeDeleted,this,&ContainerScene::deleteZone);
    }
}

void ContainerScene::addBottle(QSqlRecord rec)
{
    // Check if record is about the container
    if (rec.value("Container").toInt()==id()) {
    // If position at (0,0) find first available position with no Graphics item
        QPoint bottlePos = QPoint(rec.value("ContainerX").toInt(), rec.value("ContainerY").toInt());
        if (bottlePos.isNull()) {
            bottlePos=bottlePos+QPoint(40,15);
            while (items(bottlePos).size() >1) {
                bottlePos = bottlePos+QPoint(20,0);
            }
            rec.setValue("ContainerX",bottlePos.x());
            rec.setValue("ContainerY",15);
        }
    // Create and add new bottle
        ContainerBottle * bottle = new ContainerBottle(rec,this);
     // Don't add again a bottle in the container
        if (bottle->sublocationId()==0)
            addItem(bottle);

    // Connect signals
        connect(bottle,&ContainerBottle::rectangleDataChanged,bottleModel(),&BottleTableModel::changeContainerRectangleData);
        connect(bottle,&ContainerBottle::bottleToBeDeleted,this,&ContainerScene::bottleToBeDeleted);
        connect(bottle,&ContainerBottle::bottleToBeDeleted,this,&ContainerScene::deleteBottle);

    }
}

