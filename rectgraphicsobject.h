#ifndef RECTGRAPHICSOBJECT_H
#define RECTGRAPHICSOBJECT_H

#include "graphicsobject.h"
#include "rectanglebrushstyledialog.h"
#include <QColorDialog>
#include <QSqlRecord>
#include <QKeyEvent>
#include <QMessageBox>



class RectGraphicsObject : public GraphicsObject
{
    Q_OBJECT
public:
    RectGraphicsObject(QSqlRecord rec,QGraphicsItem *parent = Q_NULLPTR);

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void keyPressEvent(QKeyEvent *event);

    virtual void clearChildren();
    virtual void clearBottleChildren();
    virtual void clearZoneChildren();

public slots:
    void changeColor();
    void changeBrushStyle();

signals:
    void colorChanged(QColor color,int id);
    void brushStyleChanged(Qt::BrushStyle bs, int id);
    void itemToBeDeleted(int id);
};

#endif // RECTGRAPHICSOBJECT_H
