#include "containerscene.h"
#include <QDebug>

ContainerScene::ContainerScene(int containerId, Room *room, QObject *parent)
    : AbstractScene(containerId, parent)
{
    // Set SQL Tables
    setDb(room->db());
    setBottleModel(room->bottleModel());
    setZoneModel(room->zoneModel());
    setTableName("Container");
    setRoomId(room->id());
    setRoom(room);

    // Store container dimensions
       setTableRatio();

    // Add color and brush style
       QPen pen;
       pen.setWidth(10);
       pen.setColor(color());
       QBrush brush(color());
       brush.setStyle(brushStyle());

      addRect(10,10,width()-20,height()-20,pen,brush);
      setName(new GraphicsText);

   // Create Context Menu
       createContextMenu();

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

    return Qt::BrushStyle(query.value(0).toInt());
}

void ContainerScene::createContextMenu()
{
    QMenu * menu = new QMenu;
    menu->addAction(tr("Create Zone"), this,SLOT(createZone()));
    menu->addAction(tr("Create Bottle"), this,SLOT(createBottle()));
    setContextMenu(menu);
}

QRect ContainerScene::decodeToQRect(const QString &text)
{
    // Create a QRect(0,0,w,h)
    QRect rect;
    QString str = text;
    rect.setX(0);
    rect.setY(0);
    str.remove(0,str.indexOf(" ")+1);
    int n = (str.left(str.indexOf("x"))).toInt();
    rect.setWidth(n);
    str.remove(0,str.indexOf("x")+1);
    n = (str.left(str.indexOf(")"))).toInt();
    rect.setHeight(n);
    return rect;
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
    // Slot for passing the context position to CreateContainerBottle
    createContainerBottle(contextPosition());
}

ContainerBottle * ContainerScene::createContainerBottle(QPointF position, int zoneId)
{
    // Retrieve SQL record
    QSqlRecord rec = bottleModel()->record();
    int newId = bottleModel()->newId();
    rec.setValue("Id",newId);
    rec.setValue("PurchaseDate",QDate::currentDate());
    rec.setValue("Room", roomId());
    rec.setValue("Container", id());
    rec.setValue("ContainerX", position.x());
    rec.setValue("ContainerY", position.y());
    rec.setValue("RoomR",40);
    rec.setValue("ContainerR",40);

    ContainerBottle * bottle = Q_NULLPTR;
    ContainerBottle *b = new ContainerBottle(rec, this);

    // Create a new SQL record if dialog accepted and add bottle
    if (b->changeBottleData(rec,-1)) {
        bottle = addBottle(rec);
        if (bottle) {
            bottleModel()->submitAll();
            bottleModel()->select();

            // Add and position the new Bottle in the Room
            requestBottlePositioning(newId,position);
        }
    }
    delete b;
    return bottle;
}

void ContainerScene::requestBottlePositioning(int bottleId, QPointF containerPos)
{
    // Convert position thanks to ratios
    QPointF positionPointF = QPointF(containerPos.x()/ratio().x(), containerPos.y()/ratio().y());
    room()->positionBottle(bottleId, positionPointF);
}

QPointF ContainerScene::ratio() const
{
    return m_ratio;
}

void ContainerScene::setRatio(const QPointF &ratio)
{
    m_ratio = ratio;
}

Room *ContainerScene::room() const
{
    return m_room;
}

void ContainerScene::setRoom(Room *room)
{
    m_room = room;
}

int ContainerScene::roomId() const
{
    return m_roomId;
}

void ContainerScene::setRoomId(int roomId)
{
    m_roomId = roomId;
}

void ContainerScene::setTableRatio()
{
    // Prepare Query to retrieve width and height
    QSqlQuery query;
    query.prepare("SELECT Width, Height, QRect FROM Container WHERE Id = :id");
    query.bindValue(":id",id());
    query.exec();
    query.next();

    QRect rect = decodeToQRect(query.value("QRect").toString());
    setSceneRect(rect);

    qreal xRatio = width()/query.value(0).toInt();
    qreal yRatio = height()/query.value(1).toInt();

    // Set Ratio in object
    setRatio(QPointF(xRatio,yRatio));

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

ContainerBottle * ContainerScene::addBottle(QSqlRecord rec)
{
    // Check if record is about the container
    if (rec.value("Container").toInt()==id()) {
    // If position at (0,0) find first available position with no Graphics item
        QPointF bottlePos = QPointF(rec.value("ContainerX").toDouble(), rec.value("ContainerY").toDouble());
        if (bottlePos.isNull()) {
            bottlePos=bottlePos+QPointF(40,15);
            while (items(bottlePos).size() >1) {
                bottlePos = bottlePos+QPointF(20,0);
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
        return bottle;
    }
    return Q_NULLPTR;
}

