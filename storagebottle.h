#ifndef STORAGEBOTTLE_H
#define STORAGEBOTTLE_H

#include "abstractbottle.h"
class StorageDelegate;
#include "abstractstoragescene.h"
#include "zone.h"

class Zone;

class StorageBottle : public AbstractBottle
{
    Q_OBJECT
public:
    StorageBottle(QSqlRecord rec, BottleTableModel *bottleModel, QGraphicsItem *parent = Q_NULLPTR);

    int type() const;
    void changeZone(Zone *newZone);

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

signals:
    void bottlePositioningRequested(int bottleid,  QPointF changedPosition);

public slots:
   virtual void changeRectangleData();

private:
    bool fPositioning;
};

#endif // STORAGEBOTTLE_H
