#include "bottle.h"
#include <QDebug>


Bottle::Bottle(QSqlRecord rec, Room *room, QGraphicsItem *parent)
    : AbstractBottle("Room","Container",rec, parent)
{
// Set Bottle Model
setBottleModel(room->bottleModel());

// Assign the Bottle to the Container parent

int containerId = sublocationId();

   if (containerId !=0) {
  QList<QGraphicsItem *> itemList = room->items();
  Container * container;
  Container * containerItem;
  //Find the Graphics item with the correct ContainerId
  foreach (QGraphicsItem * item, itemList) {
      if (item->type()==UserType){
          containerItem = static_cast<Container*>(item);
          if (containerItem->id() == containerId) {
              container = containerItem;
              break;
          }
      }
    }
   setParentItem(container);
   }

// Position the bottle on screen
   QPointF pos = QPointF(rec.value("RoomX").toDouble(),rec.value("RoomY").toDouble());
   setPos(pos);
}


int Bottle::type() const
{
    return UserType+1;
}

void Bottle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   if (event) {
       int containerId = sublocationId();
       QList<QGraphicsItem *> cItems = collidingItems();
       int iContainer=-1;
           // Find the first container available
           for (int i=0, n=cItems.size();i<n;i++) {
               if (cItems.at(i)->type() == UserType) {
                   iContainer=i;
                   break;
               }
           }
        int newContainerId=0; // Set to 0 if no container at mouse position
           if (iContainer !=-1) {
               // Retrieve Container Container Id
               Container * container = static_cast <Container *> (cItems.at(iContainer));
               newContainerId= container->id();
           }
    if (newContainerId != containerId) {
        // Change Container
        changeContainer(newContainerId);
        // Save current container position before change
        QPointF oldContainerPos = (parentItem()==nullptr)? QPointF(): parentItem()->pos();
        if (iContainer !=-1) {
            Container * container = static_cast <Container *> (cItems.at(iContainer));
            setParentItem(container);
            // Find new position in Container position
            setPos(pos()+oldContainerPos-parentItem()->pos());}
        else {
            setParentItem(nullptr);
            // Find new position in Room position
            setPos(pos()+oldContainerPos);
        }
       }
   }
   QGraphicsObject::mouseReleaseEvent(event);
}

void Bottle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
   // Execute selected item of Context Menu
   contextMenu()->exec(event->screenPos());
}

void Bottle::changeContainer(int newContainerId)
{
    setSublocationId(newContainerId);
    bottleModel()->changeContainer(id(),newContainerId);
}

