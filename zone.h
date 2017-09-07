#ifndef ZONE_H
#define ZONE_H

#include "rectgraphicsobject.h"

class Zone : public RectGraphicsObject
{
    Q_OBJECT
public:
    Zone(QSqlRecord rec, QGraphicsItem *parent = Q_NULLPTR);

    int type() const;

signals:
    void bottlePositioningRequested(int bottleId, QPointF changedPos);
    void bottleCreationRequested(QPointF position, Zone *zone);

private slots:
    void createBottle();

public slots:
   virtual void changeRectangleData();

private:
    void clearBottleChildren();
};

#endif // ZONE_H
