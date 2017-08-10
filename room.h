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

public slots:
    void createContainer();
    void deleteContainer(int id);
    void createBottle();
    void repositionBottle(int bottleId, QPoint pos);
    void changeWineColor(int index, QColor color);
    void changeWinebrushStyle(int index, Qt::BrushStyle bs);

private:
    void addContainer(QSqlRecord rec);
    void addBottle(QSqlRecord rec);
    void createContextMenu();

private:
    ContainerTableModel *m_containerModel;

};

#endif // ROOM_H
