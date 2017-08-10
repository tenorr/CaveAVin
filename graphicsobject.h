#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <QGraphicsObject>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QPainter>

class GraphicsObject : public QGraphicsObject
{
    Q_OBJECT
public:
    GraphicsObject(QGraphicsItem *parent = Q_NULLPTR);

    int id() const;
    void setId(int id);

    QRect rect() const;
    void setRect(const QRect &rect);

    QColor color() const;
    void setColor(const QColor &color);

    Qt::BrushStyle brushStyle() const;
    void setBrushStyle(const Qt::BrushStyle &brushStyle);

    void changeRectangleSize(QPointF dPos);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:
    void rectangleDataChanged(QRect data, int id);

public slots:
   virtual void changeRectangleData();

protected:
    int m_id;
    QRect m_rect;
    QColor m_color;
    Qt::BrushStyle m_brushStyle;
};

#endif // GRAPHICSOBJECT_H
