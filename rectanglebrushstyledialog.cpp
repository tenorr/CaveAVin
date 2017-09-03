#include "rectanglebrushstyledialog.h"

RectangleBrushStyleDialog::RectangleBrushStyleDialog(QColor color, Qt::BrushStyle initialBrushstyle, QWidget *parent)
    : BrushStyleDialog(parent)
{
    setPattern(color,initialBrushstyle);
}

void RectangleBrushStyleDialog::setPattern(QColor color, Qt::BrushStyle initialBrushstyle)
{
    // Set the 5x3 rectangles
    for (int i=0;i<15;i++) {
        QPen pen = QPen(color);
        QBrush brush = QBrush(color,Qt::BrushStyle(i));
        QGraphicsRectItem *item = scene()->addRect(QRectF(0,0,RECTSIZE,RECTSIZE),pen,brush);
        item->setFlag(QGraphicsItem::ItemIsSelectable);

        // Select the current brushStyle
        if (initialBrushstyle == Qt::BrushStyle(i))
            item->setSelected(true);
        item->setPos(10+(i%3)*(RECTSIZE+10),10+(i/3)*(RECTSIZE+10));
    }
}
