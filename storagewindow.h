#ifndef STORAGEWINDOW_H
#define STORAGEWINDOW_H

class AbstractStorageScene;
#include "container.h"
#include "cellar.h"
#include "storagedelegate.h"

#include <QGraphicsView>
#include <QMainWindow>
#include <QMenuBar>

class Cellar;
class StorageDelegate;


class StorageWindow : public QMainWindow
{
    Q_OBJECT

public:
    StorageWindow(int storageId, Cellar *cellar, QWidget *parent = Q_NULLPTR);

    QGraphicsView *graphicsView() const;
    void setGraphicsView(QGraphicsView *graphicsView);

    Cellar *cellar() const;
    void setCellar(Cellar *cellar);

    QMenu **menus() const;
    void setMenus(QMenu **menus);

    AbstractStorageScene *storageScene() const;
    void setStorageScene(AbstractStorageScene *storageScene);

    int storageId() const;
    void setStorageId(int storageId);

    StorageDelegate *delegate();
    void setDelegate(StorageDelegate *delegate);

protected:
    virtual void moveEvent(QMoveEvent *event);

private:
    QRect getGeometryFromDatabase(int storageId);
    AbstractStorageScene * findStorageType();

private:
    QMenu ** m_menus;
    QGraphicsView *m_graphicsView;
    AbstractStorageScene *m_storageScene;
    Cellar *m_cellar;
    int m_storageId;
    StorageDelegate *m_delegate;
};

#endif // STORAGEWINDOW_H
