#ifndef STORAGE_H
#define STORAGE_H

#include "cellar.h"
#include "rectgraphicsobject.h"

class Cellar;

class Storage : public RectGraphicsObject
{
Q_OBJECT
public:
    Storage(QSqlRecord rec, Cellar *cellar, QGraphicsItem *parent = Q_NULLPTR);

    int storageType() const;
    void setStorageType(int storageType);

    int type() const;

    Cellar *cellar() const;
    void setCellar(Cellar *cellar);

protected:
    virtual void changeRectangleSize(QPointF dPos);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    void clearBottleChildren();
    void clearZoneChildren();

private:
    int m_storageType;
    Cellar* m_cellar;

};

#endif // STORAGE_H
