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
    void bottlePositionChanged(int bottleId, QPointF vector);

protected:
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

};

#endif // ZONE_H
