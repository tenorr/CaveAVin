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

    QRectF rect() const;
    void setRect(const QRectF &rect);

    QColor color() const;
    void setColor(const QColor &color);

    Qt::BrushStyle brushStyle() const;
    void setBrushStyle(const Qt::BrushStyle &brushStyle);

    virtual void changeRectangleSize(QPointF dPos);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:
    void rectangleDataChanged(QRectF data, int id);

public slots:
   virtual void changeRectangleData();

protected:
    int m_id;
    QRectF m_rect;
    QColor m_color;
    Qt::BrushStyle m_brushStyle;
};

#endif // GRAPHICSOBJECT_H
