#include "containerbottle.h"

ContainerBottle::ContainerBottle(QSqlRecord rec, ContainerScene *containerscene, QGraphicsItem *parent)
    :AbstractBottle("Container","Zone",rec,parent)
{
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
       QPoint pos = QPoint(rec.value("ContainerX").toInt(),rec.value("ContainerY").toInt());
       setPos(pos);
}

int ContainerBottle::type() const
{
    return UserType+3;
}

void ContainerBottle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event) {
        int zoneId = sublocationId();
        QList<QGraphicsItem *> cItems = collidingItems();
        int iZone=-1;
            // Find the first zone available
            for (int i=0, n=cItems.size();i<n;i++) {
                if (cItems.at(i)->type() == UserType+2) {
                    iZone=i;
                    break;
                }
            }
         int newZoneId=0; // Set to 0 if no zone at mouse position
            if (iZone !=-1) {
                // Retrieve Container Container Id
                Zone * zone = static_cast <Zone *> (cItems.at(iZone));
                newZoneId= zone->id();
            }
     if (newZoneId != zoneId) {
         // Change Zone
          changeZone(newZoneId);
         // Save current Zone position before change
         QPointF oldZonePos = (parentItem()==nullptr)? QPointF(): parentItem()->pos();
         if (iZone !=-1) {
             Zone * zone = static_cast <Zone *> (cItems.at(iZone));
             setParentItem(zone);
             // Find new position in Zone position
             setPos(pos()+oldZonePos-parentItem()->pos());}
         else {
             setParentItem(nullptr);
             // Find new position in Container position
             setPos(pos()+oldZonePos);
         }
        }
    }
    QGraphicsObject::mouseReleaseEvent(event);
}

void ContainerBottle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
   // Execute selected item of Context Menu
    contextMenu()->exec(event->screenPos());
}

void ContainerBottle::changeZone(int newZoneId)
{
    setSublocationId(newZoneId);
    bottleModel()->changeZone(id(),newZoneId);
}
