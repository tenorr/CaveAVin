#ifndef CONTAINERSCENE_H
#define CONTAINERSCENE_H

#include "abstractscene.h"
#include "containerbottle.h"
#include "room.h"
#include "zone.h"

class Room;

class ContainerScene : public AbstractScene
{
    Q_OBJECT
public:
    ContainerScene(int containerId, Room &room,QObject *parent = Q_NULLPTR);

    void addZone(QSqlRecord rec);
    void addBottle(QSqlRecord rec);

    void setRatio();

private:
    QColor color();
    Qt::BrushStyle brushStyle();
    void createContextMenu();

public slots:
    void createZone();
    void deleteZone(int zoneId);
    void createBottle();

signals:
    void bottleToBeDeleted(int bottleId);

};

#endif // CONTAINERSCENE_H
