#include "container.h"

Container::Container(int containerId, QObject *parent)
    :AbstractStorageScene (containerId,parent)
{
    createContextMenu();
    delegate()->setConnectCellar();

    // Add color and brush style and paint the rectangle
    QPen pen;
    pen.setWidth(10);
    pen.setColor(color());
    QBrush brush(color());
    brush.setStyle(brushStyle());

    addRect(10,10,width()-20,height()-20,pen,brush);

    // Add Container Name
    setName(new GraphicsText);

    // Populate Container with zones
    foreach (Zone * zone, delegate()->zones()) {
        addItem(zone);
        connect(zone,&Zone::bottleCreationRequested,this,&Container::createBottleFromZone);
    }

     // Populate Container with bottles
    foreach (StorageBottle* bottle, delegate()->bottles()) {
        if (bottle->sublocationId()==0) {
            solveZeroPositionIssue(bottle);
            addItem(bottle);
         }
   }
}

void Container::createZone()
{
    // Create Zone and Database entry
    Zone *newZone = delegate()->createZone(contextPosition());
    connect(newZone,&Zone::bottleCreationRequested,this,&Container::createBottleFromZone);

    // Add Item on scene
    addItem(newZone);
}

void Container::createBottle()
{
   StorageBottle *bottle = delegate()->createBottle(contextPosition());

   // If bottle created add item on scene
   if (bottle) {
       addItem(bottle);
       // Reposition on cellar
       delegate()->positionBottleOnCellar(bottle->id(),contextPosition());}
}

void Container::createBottleFromZone(QPointF position, Zone *zone)
{
    // Create bottle (converted into zone position by delegate
    StorageBottle *bottle = delegate()->createBottle(position,zone);
    if (bottle)
        // Reposition on cellar
        delegate()->positionBottleOnCellar(bottle->id(),position);
}

void Container::createContextMenu()
{
    QMenu * menu = new QMenu;
    menu->addAction(tr("Create Zone"), this, SLOT(createZone()));
    menu->addAction(tr("Create Bottle"), this, SLOT(createBottle()));
    setContextMenu(menu);
}

void Container::solveZeroPositionIssue(StorageBottle *bottle)
{
    QPointF bottlePos = bottleModel()->storagePosition(bottle->id());
    // Test if position at (0,0)
    if (bottlePos.isNull()) {
    // Find the first available position with no Graphics item
     // WARNING don't take into account the width of the scene
        bottlePos=bottlePos+QPointF(40,15);
        while (items(bottlePos).size() >1)
            bottlePos = bottlePos + QPointF(20,0);
        bottleModel()->setStoragePosition(bottle->id(), QPointF(bottlePos.x(),15));
        bottle->setPos(bottlePos);
    }
}
