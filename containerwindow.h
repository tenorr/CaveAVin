#ifndef CONTAINERWINDOW_H
#define CONTAINERWINDOW_H

#include "containerscene.h"
#include "room.h"

#include <QGraphicsView>
#include <QMainWindow>
#include <QMenuBar>

class Room;
class ContainerScene;

class ContainerWindow : public QMainWindow
{
    Q_OBJECT

public:
    ContainerWindow(int containerId, Room *room, QWidget *parent = Q_NULLPTR);

    QGraphicsView *graphicsView() const;
    void setGraphicsView(QGraphicsView *graphicsView);

    Room *room() const;
    void setRoom(Room *room);

    QMenu **menus() const;
    void setMenus(QMenu **menus);

    ContainerScene *containerScene() const;
    void setContainerScene(ContainerScene *containerScene);

    int containerId() const;
    void setContainerId(int containerId);

protected:
    virtual void moveEvent(QMoveEvent *event);

private:
    QRect getGeometryFromDatabase(int containerId);
    QString encodeFromQRect(const QRect rect);
    QRect decodeToQRect(const QString &text);

private:
    QMenu ** m_menus;
    QGraphicsView *m_graphicsView;
    ContainerScene *m_containerScene;
    Room *m_room;
    int m_containerId;
};

#endif // CONTAINERWINDOW_H
