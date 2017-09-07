#include "storagebottle.h"
#include <QDebug>

StorageBottle::StorageBottle(QSqlRecord rec, BottleTableModel *bottleModel, QGraphicsItem *parent)
    :AbstractBottle("Storage","Zone",rec,parent)
{
    fPositioning = false; // Don't position bottle on the cellar during creation

    // Set Bottle Model
    setBottleModel(bottleModel);

    // Position the bottle on screen
       QPointF pos = QPointF(rec.value("StorageX").toDouble(),rec.value("StorageY").toDouble());
       setPos(pos);
       fPositioning = true;
}

int StorageBottle::type() const
{
    return UserType+3;
}

void StorageBottle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (!event->modifiers().testFlag(Qt::ShiftModifier)) { // if Shift key is not down
        // Find in what zone the mouse is released

        QList<QGraphicsItem *> cItems = collidingItems();
        int iZone=-1; // Position of item in colliding items or -1 if no collision

            // Find the first colliding zone available
            for (int i=0, n=cItems.size();i<n;i++) {
                if (cItems.at(i)->type() == UserType+2) {
                    iZone=i;
                    break;
                }
            }
        int currentZoneId = sublocationId();

        // Define new zone id (0 if none)
        Zone * newZone = (iZone !=-1)? static_cast <Zone *>(cItems.at(iZone)) : Q_NULLPTR;
        int newZoneId = (newZone)? newZone->id() : 0; // Set to 0 if no zone at mouse position

         // Change Zone if so
         if (newZoneId != currentZoneId)
             changeZone(newZone);
    }
    QGraphicsObject::mouseReleaseEvent(event);
}

void StorageBottle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
   // Execute selected item of Context Menu
    contextMenu()->exec(event->screenPos());
}

void StorageBottle::changeZone(Zone* newZone)
{
    int newZoneId = (newZone)? newZone->id() : 0;

    // Change Zone Id in Bottle Table
    setSublocationId(newZoneId);
    bottleModel()->changeZone(id(),newZoneId);

    // Recalculate the position of the bottle in the Zone while forbiding the repositioning of the bottle in the Cellar
     fPositioning = false;
     QPointF newZonePos = (newZone)? newZone->scenePos() : QPointF(0,0);
     QPointF currentPosition = scenePos();
     setParentItem(newZone);
     setPos(currentPosition-newZonePos);
     fPositioning =true;
}

void StorageBottle::changeRectangleData()
{
    if (fPositioning)
         emit bottlePositioningRequested(id(), scenePos());

        GraphicsObject::changeRectangleData();
}
