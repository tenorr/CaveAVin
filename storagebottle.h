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

    int type() const; // UserType+3;
    void changeZone(Zone *newZone);

    int rackElement() const;
    void setRackElement(int rackElement);

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

signals:
    void bottlePositioningRequested(int bottleid,  QPointF changedPosition);
    void rackElementReached(int bottleid, int rackElement);

public slots:
   virtual void changeRectangleData();

private:
    bool fPositioning;
    int m_rackElement;
};

#endif // STORAGEBOTTLE_H
