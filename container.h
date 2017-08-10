#ifndef CONTAINER_H
#define CONTAINER_H


#include "room.h"
#include "containerwindow.h"
#include "rectgraphicsobject.h"


class Room;

class Container : public RectGraphicsObject
{
Q_OBJECT
public:
    Container(QSqlRecord rec, Room *room, QGraphicsItem *parent = Q_NULLPTR);

    int containerType() const;
    void setContainerType(int containerType);

    int type() const;

    Room *room() const;
    void setRoom(Room *room);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    void clearBottleChildren();
    void clearZoneChildren();

private:
    int m_containerType;
    Room* m_room;

};

#endif // CONTAINER_H
