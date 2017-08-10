#ifndef ABSTRACTBOTTLE_H
#define ABSTRACTBOTTLE_H

#include "bottletablemodel.h"
#include "bottledialog.h"
#include "graphicsobject.h"
#include <QPainter>
#include <QSqlQuery>


class AbstractBottle : public GraphicsObject
{
    Q_OBJECT

public:
    AbstractBottle(QString locationName, QString subLocationName,  QSqlRecord rec, QGraphicsItem *parent = Q_NULLPTR);

    QString locationName() const;
    void setLocationName(const QString &locationName);

    int locationId() const;
    void setLocationId(int locationId);

    int sublocationId() const;
    void setSublocationId(int sublocationId);

    int wineType() const;
    void setWineType(int wineType);

    BottleTableModel *bottleModel() const;
    void setBottleModel(BottleTableModel *bottleModel);

    QMenu *contextMenu() const;
    void setContextMenu(QMenu *contextMenu);

    bool changeBottleData(QSqlRecord &rec, int row);

    int findWineTypeOf(int wineId);

public slots:
    void changeData();
    void drinkBottle();

signals:
    void bottleToBeDeleted(int id);

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    void changeRectangleData();

protected:
    QString m_locationName;
    int m_locationId;
    int m_sublocationId;
    int m_wineType;
     BottleTableModel *m_bottleModel;
     QMenu *m_contextMenu;

};

#endif // ABSTRACTBOTTLE_H
