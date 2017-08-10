#ifndef ABSTRACTSCENE_H
#define ABSTRACTSCENE_H

#include "bottletablemodel.h"
#include "graphicstext.h"
#include "zonetablemodel.h"
#include "abstractbottle.h"

#include <QGraphicsScene>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QDateEdit>


class AbstractScene : public QGraphicsScene
{
    Q_OBJECT
public:
    AbstractScene(int id, QObject *parent = Q_NULLPTR);
    ~AbstractScene();

    int id() const;
    void setId(int id);

    GraphicsText *name() const;
    void setName(GraphicsText *name);

    QString tableName() const;
    void setTableName(const QString &tableName);

    QSqlDatabase db() const;
    void setDb(const QSqlDatabase &db);

    BottleTableModel *bottleModel() const;
    void setBottleModel(BottleTableModel *bottleModel);

    QPointF contextPosition() const;
    void setContextPosition(const QPointF &contextPosition);

    QMenu *contextMenu() const;
    void setContextMenu(QMenu *contextMenu);

    ZoneTableModel *zoneModel() const;
    void setZoneModel(ZoneTableModel *zoneModel);

public slots:
    void setName(QString text);
    void deleteBottle(int bottleId);

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent);
    virtual void createContextMenu() =0;

protected:
    int m_id;
    GraphicsText *m_name;
    QString m_tableName;
    QSqlDatabase m_db;
    BottleTableModel *m_bottleModel;
    ZoneTableModel *m_zoneModel;
    QMenu *m_contextMenu;
    QPointF m_contextPosition;
};

#endif // ABSTRACTSCENE_H
