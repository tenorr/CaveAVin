#ifndef CONTAINERBOTTLE_H
#define CONTAINERBOTTLE_H

#include "abstractbottle.h"
#include "containerscene.h"
#include "zone.h"

class ContainerScene;

class ContainerBottle : public AbstractBottle
{
    Q_OBJECT
public:
    ContainerBottle(QSqlRecord rec, ContainerScene *containerscene,QGraphicsItem *parent = Q_NULLPTR);

    int type() const;
    void changeZone(Zone *newZone);

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

public slots:
   virtual void changeRectangleData();

private:
    bool fPositioning;
};

#endif // CONTAINERBOTTLE_H
