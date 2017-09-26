#ifndef RACKELEMENT_H
#define RACKELEMENT_H

#include <QGraphicsItem>
#include <QPainter>

#define ITEM_NUMBER 0


class RackElement : public QAbstractGraphicsShapeItem
{
public:
    RackElement(int number, bool fEnabled, QGraphicsItem *parent = Q_NULLPTR);

    virtual int type() const; // UserType+4

    QRectF rect() const;
    void setRect(const QRectF &rect);

    void enableElement(bool fEnabled = true);
    bool isEnabled();

    int number();
    void setNumber(int number);

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;

private:
    QRectF m_rect;

};

#endif // RACKELEMENT_H
