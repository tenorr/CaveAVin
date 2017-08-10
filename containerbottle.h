#ifndef CONTAINERBOTTLE_H
#define CONTAINERBOTTLE_H

#include "abstractbottle.h"
#include "containerscene.h"

class ContainerScene;

class ContainerBottle : public AbstractBottle
{
    Q_OBJECT
public:
    ContainerBottle(QSqlRecord rec, ContainerScene *containerscene,QGraphicsItem *parent = Q_NULLPTR);

    int type() const;

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void changeZone(int newZoneId);

};

#endif // CONTAINERBOTTLE_H
