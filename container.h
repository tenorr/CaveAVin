#ifndef CONTAINER_H
#define CONTAINER_H

#include "abstractstoragescene.h"

class StorageDelegate;
class Zone;

class Container : public AbstractStorageScene
{
    Q_OBJECT

public:
    Container(int containerId, QObject *parent = Q_NULLPTR);

public slots:
    void createZone();
    void createBottle();
    void createBottleFromZone(QPointF position, Zone *zone);

private:
    virtual void createContextMenu();
};

#endif // CONTAINER_H
