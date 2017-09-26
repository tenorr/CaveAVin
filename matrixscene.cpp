#include "matrixscene.h"
#include <QDebug>

MatrixScene::MatrixScene(int storageId, QObject *parent)
    : AbstractStorageScene(storageId,parent)
{
    // Disconnect storage & cellar
    delegate()->setConnectCellar(false);

    // Add color and brush style and paint the rectangle
    QPen pen;
    pen.setWidth(10);
    pen.setColor(color());
    QBrush brush(color());
    brush.setStyle(brushStyle());
    addRect(10,10,width()-20,height()-20,pen,brush);

    // Add Container Name
    setName(new GraphicsText);

    // Retrieve rowCount, columnCount and rackHint;
    int rowCount = delegate()->storageRowCount();
    int columnCount = delegate()->storageColumnCount();
    int rackHint = delegate()->rackHint();

   // Find top left corner position of rack elements
    QPointF rackPosition = QPointF((width()-rackHint*columnCount)/2,40+(height()-rackHint*rowCount)/2);

    // Retrieve ELements and position them
    foreach (RackElement * item,  delegate()->rackElements()) {
        int n = item->number()-1;
        item->setPos(rackPosition + QPointF((n % columnCount)*rackHint,(n/columnCount)*rackHint));
        addItem(item);
    }
    // Populate storage with bottles and connect signal
   foreach (StorageBottle* bottle, delegate()->bottles()) {
           positionBottleOnRackElement(bottle);
           connect(bottle,&StorageBottle::rackElementReached,this,&MatrixScene::manageRackElement);
           addItem(bottle);
  }
}

void MatrixScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent)
{
    if (contextMenuEvent) {
        QMenu menu;
        // Find if an item is under the mouse
       QGraphicsItem * item = itemAt(contextMenuEvent->scenePos(),QTransform());
       if (item) {
           setContextPosition(contextMenuEvent->scenePos());
           if (item->type() == QGraphicsItem::UserType+5) { // Rack Element         
               // Enable or Disable item
               if (static_cast<MatrixRackElement *>(item)->isEnabled()) {
                   menu.addAction(tr("Add Bottle"),this, SLOT(addBottle()));
                   menu.addAction(tr("Disable Element"),this, SLOT(setElementEnabled()));}
               else
                   menu.addAction(tr("Enable Element"),this, SLOT(setElementEnabled()));
            }
            else if (item->type() == QGraphicsItem::UserType+3) { // Storage Bottle
              menu.addAction(tr("Change Data"), this, SLOT(changeBottleData()));
              menu.addAction(tr("Drink Bottle"), this, SLOT(drinkBottle()));
             }
      // Add last action (circular - rectangular) and Execute Context Menu
      menu.addAction((isCircular())? tr("-> Rectangular"): tr("-> Circular"),this,SLOT(changeElementShape()));
      menu.exec(contextMenuEvent->screenPos());
      }
    }
}

bool MatrixScene::isCircular()
{
    MatrixRackElement * item = rackElement(1); // Look first element circularity

    return (item)? rackElement(1)->isCircular() : true;
}

bool MatrixScene::isDisabledItem(int rackElement)
{
   if (rackElement >0)
        return !delegate()->rackElementItem(rackElement)->isEnabled();
   return false;
}

void MatrixScene::positionBottleOnRackElement(StorageBottle *bottle)
{
    if (bottle) {
        if (bottle->rackElement()==0)
             bottle->setPos(firstAvailableZeroPosition());
        else {
            bottle->setPos(delegate()->rackElementItem(bottle->rackElement())->pos());
        }
    }
}

MatrixRackElement *MatrixScene::rackElement(int number)
{
    return static_cast<MatrixRackElement *>(delegate()->rackElementItem(number));
}

void MatrixScene::manageRackElement(int bottleId, int newRackElement)
{
    // Find bottle data in delegate bottle
    StorageBottle * bottle = delegate()->findBottle(bottleId);
    if (bottle)
    {
        // Find bottle rack element
        int bottleRackElement = bottle->rackElement();
        // if new rack is different
        if (bottleRackElement != newRackElement) {
            // Find if rack is not disabled
            if (isDisabledItem(newRackElement)) {
                // Reposition the bottle in its original state
                    positionBottleOnRackElement(bottle);
            }
            // Else position on the rack element and update bottle database
            else {
                bottle->setRackElement(newRackElement);
                positionBottleOnRackElement(bottle);
                bottleModel()->changeRackElement(bottleId,newRackElement);
            }
        }
    }
}

void MatrixScene::setElementEnabled()
{
    // Retrieve element
    MatrixRackElement * item = static_cast<MatrixRackElement *>(itemAt(contextPosition(),QTransform()));
    if (item)
       delegate()->setRackElementEnabled(item,!item->isEnabled());
}

void MatrixScene::changeElementShape()
{
   delegate()->setRackCircular(!isCircular());
}

void MatrixScene::addBottle()
{
    // Find rackElement position
    MatrixRackElement * rackElement = static_cast<MatrixRackElement *>(itemAt(contextPosition(),QTransform()));

    if (rackElement) {
        StorageBottle *bottle = delegate()->createBottle(rackElement->pos(),Q_NULLPTR,100,rackElement->number());
        // If bottle created add item on scene
        if (bottle) {
            connect(bottle,&StorageBottle::rackElementReached,this,&MatrixScene::manageRackElement);
            positionBottleOnRackElement(bottle);
            addItem(bottle);
            // Reposition on cellar
            delegate()->positionBottleOnCellar(bottle->id(),contextPosition());
        }
    }
}

void MatrixScene::changeBottleData()
{
     StorageBottle * bottle = static_cast<StorageBottle *>(itemAt(contextPosition(),QTransform()));
     if (bottle)
        bottle->changeData();
}

void MatrixScene::drinkBottle()
{
    StorageBottle * bottle = static_cast<StorageBottle *>(itemAt(contextPosition(),QTransform()));
    if (bottle)
        bottle->drinkBottle();
}
