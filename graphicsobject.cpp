#include "graphicsobject.h"

GraphicsObject::GraphicsObject(QGraphicsItem *parent)
    :QGraphicsObject(parent)
{
    // Set the item selectable, movable and focusable
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsFocusable);

    // Connect the position signals with the rectangleDataChanged slot
    connect(this,&GraphicsObject::xChanged,this,&GraphicsObject::changeRectangleData);
    connect(this,&GraphicsObject::yChanged,this,&GraphicsObject::changeRectangleData);
}

int GraphicsObject::id() const
{
    return m_id;
}

void GraphicsObject::setId(int id)
{
    m_id = id;
}

QRectF GraphicsObject::rect() const
{
    return m_rect;
}

void GraphicsObject::setRect(const QRectF &rect)
{
    m_rect = rect;
}

QColor GraphicsObject::color() const
{
    return m_color;
}

void GraphicsObject::setColor(const QColor &color)
{
    m_color = color;
}

Qt::BrushStyle GraphicsObject::brushStyle() const
{
    return m_brushStyle;
}

void GraphicsObject::setBrushStyle(const Qt::BrushStyle &brushStyle)
{
    m_brushStyle = brushStyle;
}

void GraphicsObject::changeRectangleSize(QPointF dPos)
{
   // Calculate new size and limit to 20x20
     QSizeF newSize = rect().size()+QSizeF(dPos.x(), dPos.y());
        if (newSize.height()<20)
            newSize.setHeight(20);
        if (newSize.width()<20)
            newSize.setWidth(20);
        // Update Rectangle Size and emit signal
        m_rect.setSize(newSize);
        changeRectangleData();

        // Prepares the item for a geometry change and update the scene
        prepareGeometryChange();
}

void GraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event)
    {
        // Intercept event if Shift key modifier

        if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            // Change Rectangle size
            QPointF dPos = event->pos()-event->lastPos();
            changeRectangleSize(dPos);
        }
        else QGraphicsObject::mouseMoveEvent(event);
    }
}

void GraphicsObject::changeRectangleData()
{
    QRectF data;
    data.setX((x()<10)? 10: x());
    data.setY((y()<10)? 10: y());
    data.setSize(rect().size());

    emit rectangleDataChanged(data, id());
}
