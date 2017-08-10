#ifndef BOTTLE_H
#define BOTTLE_H

#include "abstractbottle.h"
#include "room.h"

#include <QSqlRecord>

class Room;

class Bottle : public AbstractBottle
{
Q_OBJECT
public:
    Bottle(QSqlRecord rec, Room *room, QGraphicsItem *parent = Q_NULLPTR);

    int type() const;

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    void changeContainer(int newContainerId);

};

#endif // BOTTLE_H
