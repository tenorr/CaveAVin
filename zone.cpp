#include "containerbottle.h"
#include "zone.h"
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
    ContainerScene *containerScene = static_cast<ContainerScene *>(scene());
    // Create the Container Bottle
    ContainerBottle * bottle = containerScene->createContainerBottle(contextPosition(),id());
    // Set container Bottle to Zone
    bottle->changeZone(this);
}

void Zone::changeRectangleData()
{
    // Find ChildItem of Type Container Bottle
    ContainerScene * containerScene = static_cast<ContainerScene *>(scene());
    QList<QGraphicsItem *> bottleList = childItems();
    foreach (QGraphicsItem * item, bottleList) {
        if (item->type() == UserType+3) {
            ContainerBottle * bottle = static_cast<ContainerBottle *>(item);
            // Get bottle Id and container position
            int bottleId = bottle->id();
            QPointF changedPos = bottle->scenePos();
            // Position the bottle in the Room accordingly
            containerScene->requestBottlePositioning(bottleId, changedPos);
        }
    }
    GraphicsObject::changeRectangleData();
}
