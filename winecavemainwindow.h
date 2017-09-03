#ifndef WINECAVEMAINWINDOW_H
#define WINECAVEMAINWINDOW_H

#include "room.h"
#include "containertablemodel.h"
#include "bottletablemodel.h"

#include <QMainWindow>
#include <QApplication>
#include <QScreen>
#include <QMenu>
#include <QMenuBar>
#include <QGraphicsView>
#include <QSqlDatabase>

#include "winetypedialog.h"

#define VERSION "1.0"

class WineCaveMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WineCaveMainWindow(QWidget *parent = 0);
    ~WineCaveMainWindow();

QMenu **menus() const;
void setMenus(QMenu **menus);

QGraphicsView *graphicsView() const;
void setGraphicsView(QGraphicsView *graphicsView);

Room *room() const;
void setRoom(Room *room);

QSqlDatabase db() const;
void setDb();

ContainerTableModel *containerTableModel() const;
void setContainerTableModel(ContainerTableModel *containerTableModel);

BottleTableModel *bottleTableModel() const;
void setBottleTableModel(BottleTableModel *bottleTableModel);

ZoneTableModel *zoneTableModel() const;
void setZoneTableModel(ZoneTableModel *zoneTableModel);

public slots:
void defineWineTypes();

private:
QMenu ** m_menus;
QGraphicsView *m_graphicsView;
Room *m_room;
QSqlDatabase m_db;
ContainerTableModel *m_containerTableModel;
BottleTableModel *m_bottleTableModel;
ZoneTableModel * m_zoneTableModel;

};

#endif // WINECAVEMAINWINDOW_H
