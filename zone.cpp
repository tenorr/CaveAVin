#include "zone.h"
#include "storagebottle.h"
#include <QDebug>

Zone::Zone(QSqlRecord rec, QGraphicsItem *parent)
   : RectGraphicsObject(rec, parent)
{
    contextMenu()->addAction(tr("Add Bottle"),this,SLOT(createBottle()));
}

int Zone::type() const
{
    return UserType+2;
}

void Zone::createBottle()
{
    emit bottleCreationRequested(contextPosition(), this);
}

void Zone::changeRectangleData()
{
    // Find ChildItem of Type Container Bottle
    QList<QGraphicsItem *> bottleList = childItems();
    foreach (QGraphicsItem * item, bottleList) {
        if (item->type() == UserType+3) {
            StorageBottle * bottle = static_cast<StorageBottle *>(item);
            // Get bottle Id and storage position
            int bottleId = bottle->id();
            QPointF changedPos = bottle->scenePos();
            // emit signal to Position the bottle in the Cellar accordingly
            emit bottlePositioningRequested(bottleId, changedPos);
        }
    }
    GraphicsObject::changeRectangleData();
}

void Zone::clearBottleChildren()
{
   QList<QGraphicsItem *> bottleItems = childItems();
   foreach (QGraphicsItem * item, bottleItems) {
    if (item->type()==UserType+3) {
       StorageBottle *bottle = static_cast<StorageBottle *>(item);
       bottle->setSublocationId(0);
       bottle->setParentItem(0);
       QPointF bPos = bottle->pos()+pos();
       bottle->setPos(bPos);
       bottle->bottleModel()->changeZone(bottle->id(),0);
    }
   }
}
