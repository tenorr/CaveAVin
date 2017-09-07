#ifndef WINECAVEMAINWINDOW_H
#define WINECAVEMAINWINDOW_H

#include "cellar.h"
#include "storagetablemodel.h"
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

Cellar *cellar() const;
void setCellar(Cellar *cellar);

QSqlDatabase db() const;
void setDb();

StorageTableModel *storageTableModel() const;
void setStorageTableModel(StorageTableModel *storageTableModel);

BottleTableModel *bottleTableModel() const;
void setBottleTableModel(BottleTableModel *bottleTableModel);

ZoneTableModel *zoneTableModel() const;
void setZoneTableModel(ZoneTableModel *zoneTableModel);

public slots:
void defineWineTypes();


private:
QMenu ** m_menus;
QGraphicsView *m_graphicsView;
Cellar *m_cellar;
QSqlDatabase m_db;
StorageTableModel *m_storageTableModel;
BottleTableModel *m_bottleTableModel;
ZoneTableModel * m_zoneTableModel;

};

#endif // WINECAVEMAINWINDOW_H
