#ifndef GRAPHICSTEXT_H
#define GRAPHICSTEXT_H

#include <QGraphicsTextItem>
#include <QPainter>
#include "graphicstextdialog.h"

class GraphicsText : public QGraphicsTextItem
{
    Q_OBJECT
public:
    GraphicsText(QGraphicsItem *parent = Q_NULLPTR);

signals:
    nameChanged(QString newName);

protected:
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

#endif // GRAPHICSTEXT_H
