#include "bottle.h"
#include <QDebug>


Bottle::Bottle(QSqlRecord rec, Cellar *cellar, QGraphicsItem *parent)
    : AbstractBottle("Cellar","Storage",rec, parent)
{
// Set Bottle Model
setBottleModel(cellar->bottleModel());

// Assign the Bottle to the Storage parent

int storageId = sublocationId();

   if (storageId !=0) {
  QList<QGraphicsItem *> itemList = cellar->items();
  Storage * storage;
  Storage * storageItem;
  //Find the Graphics item with the correct StorageId
  foreach (QGraphicsItem * item, itemList) {
      if (item->type()==UserType){
          storageItem = static_cast<Storage*>(item);
          if (storageItem->id() == storageId) {
              storage = storageItem;
              break;
          }
      }
    }
   setParentItem(storage);
   }

// Position the bottle on screen
   QPointF pos = QPointF(rec.value("CellarX").toDouble(),rec.value("CellarY").toDouble());
   setPos(pos);
}


int Bottle::type() const
{
    return UserType+1;
}

void Bottle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   if (event) {
       int storageId = sublocationId();
       QList<QGraphicsItem *> cItems = collidingItems();
       int iStorage=-1;
           // Find the first storage available
           for (int i=0, n=cItems.size();i<n;i++) {
               if (cItems.at(i)->type() == UserType) {
                   iStorage=i;
                   break;
               }
           }
        int newStorageId=0; // Set to 0 if no storage at mouse position
           if (iStorage !=-1) {
               // Retrieve storage Storage Id
               Storage * storage = static_cast <Storage *> (cItems.at(iStorage));
               newStorageId= storage->id();
           }
    if (newStorageId != storageId) {
        // Change storage
        changeStorage(newStorageId);
        // Save current storage position before change
        QPointF oldStoragePos = (parentItem()==nullptr)? QPointF(): parentItem()->pos();
        if (iStorage !=-1) {
            Storage * storage = static_cast <Storage *> (cItems.at(iStorage));
            setParentItem(storage);
            // Find new position in Storage position
            setPos(pos()+oldStoragePos-parentItem()->pos());}
        else {
            setParentItem(nullptr);
            // Find new position in Cellar position
            setPos(pos()+oldStoragePos);
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

void Bottle::changeStorage(int newStorageId)
{
    setSublocationId(newStorageId);
    bottleModel()->changeStorage(id(),newStorageId);
}

