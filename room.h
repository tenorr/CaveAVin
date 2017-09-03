#ifndef ROOM_H
#define ROOM_H

#include "abstractscene.h"
#include "container.h"
#include "containertablemodel.h"
#include "bottledialog.h"
#include "bottle.h"

class Bottle;

class Room : public AbstractScene
{
    Q_OBJECT
public:
    Room(ContainerTableModel *containerModel, BottleTableModel *bottleModel, ZoneTableModel *zoneModel,int id=1, QObject *parent = Q_NULLPTR);
    ~Room();

    ContainerTableModel *containerModel() const;
    void setContainerModel(ContainerTableModel *containerModel);
    void positionBottle(int bottleId,QPointF position);
    void moveBottle(int bottleId, QPointF move);

public slots:
    void createContainer();
    void deleteContainer(int id);
    void createBottle();

private:
    void addContainer(QSqlRecord rec);
    void addBottle(QSqlRecord rec);
    Bottle * findBottle(int bottleId);
    Bottle *createBottleFromTableData(int bottleId);
    void createContextMenu();

private:
    ContainerTableModel *m_containerModel;

};

#endif // ROOM_H
