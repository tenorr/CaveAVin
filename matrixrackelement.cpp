#include "matrixrackelement.h"

MatrixRackElement::MatrixRackElement(int number, bool isCircular, bool fEnabled, QGraphicsItem *parent)
    : RackElement(number, fEnabled, parent)
{
    setCircular(isCircular);
}

int MatrixRackElement::type() const
{
    return UserType+5;
}

void MatrixRackElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

  painter->setPen(pen());
  painter->setBrush(brush());

  if (fCircular)
    painter->drawEllipse(rect());
  else
    painter->drawRect(rect());

        Q_UNUSED(option)
        Q_UNUSED(widget)
}

bool MatrixRackElement::isCircular() const
{
    return fCircular;
}

void MatrixRackElement::setCircular(bool isCircular)
{
    fCircular = isCircular;
    prepareGeometryChange();
}
