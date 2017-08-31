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

    int roomId() const;
    void setRoomId(int roomId);

private:
    QColor color();
    Qt::BrushStyle brushStyle();
    void createContextMenu();
    QRect decodeToQRect(const QString &text);

public slots:
    void createZone();
    void deleteZone(int zoneId);
    void createBottle();

signals:
    void bottleToBeDeleted(int bottleId);

private:
    int m_roomId;

};

#endif // CONTAINERSCENE_H
