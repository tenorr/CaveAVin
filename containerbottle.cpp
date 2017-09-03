#include "containerbottle.h"
#include <QDebug>

ContainerBottle::ContainerBottle(QSqlRecord rec, ContainerScene *containerscene, QGraphicsItem *parent)
    :AbstractBottle("Container","Zone",rec,parent)
{
    fPositioning = false; // Don't position bootle during creation

    // Set Bottle Model
    setBottleModel(containerscene->bottleModel());

    // Assign the Bottle to the Zone parent

    int zoneId = sublocationId();

      if (zoneId !=0) {
        QList<QGraphicsItem *> itemList = containerscene->items();
        Zone * zone;
        Zone * zoneItem;

        //Find the Graphics item with the correct ZoneId
        foreach (QGraphicsItem * item, itemList) {
            if (item->type()==UserType+2){
                zoneItem = static_cast<Zone*>(item);
                if (zoneItem->id() == zoneId) {
                    zone = zoneItem;
                    break;
                }
            }
        }
       setParentItem(zone);
       }

    // Position the bottle on screen
       QPointF pos = QPointF(rec.value("ContainerX").toDouble(),rec.value("ContainerY").toDouble());
       setPos(pos);
       fPositioning = true;
}

int ContainerBottle::type() const
{
    return UserType+3;
}

void ContainerBottle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
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

void ContainerBottle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
   // Execute selected item of Context Menu
    contextMenu()->exec(event->screenPos());
}

void ContainerBottle::changeZone(Zone* newZone)
{
    int newZoneId = (newZone)? newZone->id() : 0;

    // Change Zone Id in Bottle Table
    setSublocationId(newZoneId);
    bottleModel()->changeZone(id(),newZoneId);

    // Recalculate the position of the bottle in the Zone while forbiding the repositioning of the bottle in the room
     fPositioning = false;
     QPointF newZonePos = (newZone)? newZone->scenePos() : QPointF(0,0);
     QPointF currentPosition = scenePos();
     setParentItem(newZone);
     setPos(currentPosition-newZonePos);
     fPositioning =true;
}

void ContainerBottle::changeRectangleData()
{
    if (fPositioning) {
        ContainerScene * containerScene = static_cast<ContainerScene *>(scene());
        containerScene->requestBottlePositioning(id(), scenePos());
    }
        GraphicsObject::changeRectangleData();
}
