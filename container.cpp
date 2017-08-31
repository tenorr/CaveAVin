#include "container.h"

Container::Container(QSqlRecord rec, Room * room, QGraphicsItem *parent)
    : RectGraphicsObject(rec, parent)
{
    //Set pointer to  room
    setRoom(room);

    // Set container Type from record
    setContainerType(rec.value("Type").toInt());
}

int Container::containerType() const
{
    return m_containerType;
}

void Container::setContainerType(int containerType)
{
    m_containerType = containerType;
}

void Container::clearBottleChildren()
{
    QList<QGraphicsItem *> bottleItems = childItems();
    foreach (QGraphicsItem * item, bottleItems) {
        if (item->type()==UserType+1) {
            Bottle *bottle = static_cast<Bottle *>(item);
            bottle->setSublocationId(0);
            QPointF bPos = bottle->pos()+pos();
            bottle->setPos(bPos);
            bottle->setParentItem(0);
            bottle->bottleModel()->changeContainer(bottle->id(),0);
        }
    }
}

void Container::clearZoneChildren()
{
    room()->zoneModel()->deleteContainerZones(id());
}

Room *Container::room() const
{
    return m_room;
}

void Container::setRoom(Room *containerModel)
{
    m_room = containerModel;
}


int Container::type() const
{
    return UserType;
}

void Container::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event) {

    ContainerWindow *w = new ContainerWindow(id(),room(),static_cast<QWidget *>(room()->views()[0]));
    w->setAttribute(Qt::WA_DeleteOnClose,true);
    w->show();
    connect(w->containerScene(),&ContainerScene::bottleToBeDeleted,room(),&Room::deleteBottle);
    }
}
