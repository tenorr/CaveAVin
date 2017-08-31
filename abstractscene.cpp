#include "abstractscene.h"
#include <QDebug>

AbstractScene::AbstractScene(int id, QObject *parent)
    :QGraphicsScene(parent)
{
    setId(id);
    setSceneRect(0,0,1980,1220);
}

AbstractScene::~AbstractScene()
{
}

int AbstractScene::id() const
{
    return m_id;
}

void AbstractScene::setId(int id)
{
    m_id = id;
}

GraphicsText *AbstractScene::name() const
{
    return m_name;
}

void AbstractScene::setName(GraphicsText *name)
{
    // Find Name in dataBase
    QSqlQuery query;
    query.prepare(QString("SELECT %1Name FROM %1 WHERE Id = :id").arg(tableName()));
    query.bindValue(":id",id());
    query.exec();
    query.next();

   // Set found text
   name->setPlainText(query.value(0).toString());

   // Set Font
   QFont font;
   font.setPointSize(18);
   name->setFont(font);

   // Add to scene
   name->setPos(15,15);
   addItem(name);

   // Connect changeName signal
   connect(name,SIGNAL(nameChanged(QString)),this,SLOT(setName(QString)));
   m_name = name;
}

void AbstractScene::setName(QString text)
{
        // Update Data Base
        QSqlQuery query;
        query.prepare(QString("UPDATE %1 SET %1Name = :%1name WHERE Id = :id").arg(tableName()));
        query.bindValue(":id",id());
        query.bindValue(QString(":%1name").arg(tableName()),text);
        query.exec();
}

void AbstractScene::deleteBottle(int bottleId)
{
        bottleModel()->deleteBottle(bottleId);

        QList<QGraphicsItem *> itemList = items();
        foreach (QGraphicsItem * item, itemList) {
           if ((item->type()== QGraphicsItem::UserType +1) |(item->type()== QGraphicsItem::UserType +3)) {
                AbstractBottle *bottle = static_cast<AbstractBottle *>(item);
                if (bottle->id() == bottleId)
                    delete bottle;
                }
        }
}


void AbstractScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent)
{
    if (contextMenuEvent) {

        if (itemAt(contextMenuEvent->scenePos(),QTransform())->type()>=QGraphicsItem::UserType)
            QGraphicsScene::contextMenuEvent(contextMenuEvent);
        else {
            // Store context menu position
            setContextPosition(contextMenuEvent->scenePos());
            // Execute Context Menu
            contextMenu()->exec(contextMenuEvent->screenPos());
        }
    }
}

bool AbstractScene::event(QEvent *event)
{
    if (event->type() == QEvent::User) {
        // Convert event
        WineTypeEvent * wEvent = static_cast <WineTypeEvent *>(event);
        wineTypeEvent(wEvent);
        return true;
    }
    return QGraphicsScene::event(event);
}

void AbstractScene::wineTypeEvent(WineTypeEvent *event)
{
     // Retrieve every Graphics Item of Bottle Type
     QList<QGraphicsItem *> itemList = items();
     foreach (QGraphicsItem * item, itemList) {
         if ((item->type() == QGraphicsItem::UserType+1) || (item->type() == QGraphicsItem::UserType+3)) {
             AbstractBottle * bottle =  dynamic_cast<AbstractBottle *>(item);
             // Find if bottle has the wine Id
               if (event->wineId() !=0) {
                QSqlQuery query;
                query.prepare(QString("SELECT Wine FROM Bottle WHERE (Id = %1) AND (Wine = %2)").arg(bottle->id()).arg(event->wineId()));
                query.exec();
                if (!query.first())
                    continue;
            }
            else {
                // Look if Wine Type agrees
                if (bottle->wineType() != event->wineTypeId())
                    continue;
            }
            // Change Wine Type
            bottle->setWineType(event->wineTypeId());
        }
     }
}


ZoneTableModel *AbstractScene::zoneModel() const
{
    return m_zoneModel;
}

void AbstractScene::setZoneModel(ZoneTableModel *zoneModel)
{
    m_zoneModel = zoneModel;
}

QMenu *AbstractScene::contextMenu() const
{
    return m_contextMenu;
}

void AbstractScene::setContextMenu(QMenu *contextMenu)
{
    m_contextMenu = contextMenu;
}

QPointF AbstractScene::contextPosition() const
{
    return m_contextPosition;
}

void AbstractScene::setContextPosition(const QPointF &contextPosition)
{
    m_contextPosition = contextPosition;
}

BottleTableModel *AbstractScene::bottleModel() const
{
    return m_bottleModel;
}

void AbstractScene::setBottleModel(BottleTableModel *bottleModel)
{
    m_bottleModel = bottleModel;
}

QSqlDatabase AbstractScene::db() const
{
    return m_db;
}

void AbstractScene::setDb(const QSqlDatabase &db)
{
    m_db = db;
}

QString AbstractScene::tableName() const
{
    return m_tableName;
}

void AbstractScene::setTableName(const QString &tableName)
{
    m_tableName = tableName;
}
