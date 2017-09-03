#ifndef CONTAINERSCENE_H
#define CONTAINERSCENE_H

#include "abstractscene.h"
#include "containerbottle.h"
#include "room.h"
#include "zone.h"

class Room;
class ContainerBottle;

class ContainerScene : public AbstractScene
{
    Q_OBJECT
public:
    ContainerScene(int containerId, Room *room,QObject *parent = Q_NULLPTR);

    void addZone(QSqlRecord rec);
    ContainerBottle *addBottle(QSqlRecord rec);

    void setTableRatio();

    int roomId() const;
    void setRoomId(int roomId);

    Room *room() const;
    void setRoom(Room *room);

    QPointF ratio() const;
    void setRatio(const QPointF &ratio);

    ContainerBottle *createContainerBottle(QPointF position, int zoneId=0);
    void requestBottlePositioning(int bottleId, QPointF containerPos);

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
    QPointF m_ratio;
    Room * m_room;

};

#endif // CONTAINERSCENE_H
