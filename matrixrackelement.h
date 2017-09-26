#ifndef MATRIXRACKELEMENT_H
#define MATRIXRACKELEMENT_H

#include "rackelement.h"


class MatrixRackElement : public RackElement
{
public:
    MatrixRackElement(int number, bool isCircular, bool fEnabled = true, QGraphicsItem *parent = Q_NULLPTR);

    virtual int type() const; // UserType+5

    bool isCircular() const;
    void setCircular(bool isCircular);

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    bool fCircular;

};

#endif // MATRIXRACKELEMENT_H
