#include "circlebrushstyledialog.h"

CircleBrushStyleDialog::CircleBrushStyleDialog(QColor color, Qt::BrushStyle initialBrushstyle, QWidget *parent)
    :BrushStyleDialog(parent)
{
    setPattern(color,initialBrushstyle);
}

void CircleBrushStyleDialog::setPattern(QColor color, Qt::BrushStyle initialBrushstyle)
{
    // Set the 5x3 circles
    for (int i=0;i<15;i++) {
        QPen pen = QPen(color);
        QBrush brush = QBrush(color,Qt::BrushStyle(i));
        QGraphicsEllipseItem *item = scene()->addEllipse(QRectF(0,0,RECTSIZE,RECTSIZE),pen,brush);
        item->setFlag(QGraphicsItem::ItemIsSelectable);

        // Select the current brushStyle
        if (initialBrushstyle == Qt::BrushStyle(i))
            item->setSelected(true);

       // item->setFlag(QGraphicsItem::ItemIsFocusable);
        item->setPos(10+(i%3)*(RECTSIZE+10),10+(i/3)*(RECTSIZE+10));
    }

}
