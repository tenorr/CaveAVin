#ifndef ZONE_H
#define ZONE_H

#include "rectgraphicsobject.h"


class Zone : public RectGraphicsObject
{
    Q_OBJECT
public:
    Zone(QSqlRecord rec, QGraphicsItem *parent = Q_NULLPTR);

    int type() const;
};

#endif // ZONE_H
