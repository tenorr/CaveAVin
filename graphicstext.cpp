#include "graphicstext.h"

GraphicsText::GraphicsText(QGraphicsItem *parent)
    :QGraphicsTextItem(parent)
{

}

void GraphicsText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event) {
        // Create a new Dialog
        GraphicsTextDialog *dialog= new GraphicsTextDialog(toPlainText());
        if ((dialog->exec() == QDialog::Accepted) and (!dialog->lineEdit()->text().isEmpty())) {
            // Change text and emit signal
            setPlainText(dialog->lineEdit()->text());
            emit nameChanged(dialog->lineEdit()->text());
        }
        delete dialog;
    }
}

void GraphicsText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Set background in white
    QPen pen = painter->pen();
    painter->setBrush(Qt::white);
    painter->setPen(Qt::white);
    painter->drawRect(boundingRect());
    painter->setPen(pen);
    QGraphicsTextItem::paint(painter, option, widget);
}
