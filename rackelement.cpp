#include "rackelement.h"

RackElement::RackElement(int number, bool fEnabled, QGraphicsItem *parent)
    : QAbstractGraphicsShapeItem (parent)
{
    // Initiate pen and brush
    QPen p  = pen();
    p.setWidth(1);
    p.setColor(Qt::black);
    setPen(p);

    QBrush b = brush();
    b.setColor(Qt::black);
    setBrush(b);

    // Initiate Rack Data
    setNumber(number);
    enableElement(fEnabled);

    // Set in background
    setZValue(0);
}

int RackElement::type() const
{
    return UserType+4;
}

QRectF RackElement::rect() const
{
    return m_rect;
}

void RackElement::setRect(const QRectF &rect)
{
    m_rect = rect;
    prepareGeometryChange();
}

void RackElement::enableElement(bool fEnabled)
{
    // Change the style of the brush (Obscure if disabled)

    QBrush b = brush();
    b.setStyle((fEnabled)? Qt::NoBrush : Qt::SolidPattern);
    setBrush(b);
    prepareGeometryChange();
}

bool RackElement::isEnabled()
{
    return (brush().style() == Qt::NoBrush);
}

int RackElement::number()
{
    return data(ITEM_NUMBER).toInt();
}

void RackElement::setNumber(int number)
{
    setData(ITEM_NUMBER, number);
}

void RackElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

QRectF RackElement::boundingRect() const
{
    // Include half the pen to paint an outline
        qreal penWidth = 1;

       // Return an estimate of the area painted by the item
        return QRectF(- penWidth / 2, - penWidth / 2, rect().width()+ penWidth, rect().height() + penWidth);
}
