#ifndef RECTANGLEBRUSHSTYLEDIALOG_H
#define RECTANGLEBRUSHSTYLEDIALOG_H

#include "brushstyledialog.h"

class RectangleBrushStyleDialog : public BrushStyleDialog
{
public:
    RectangleBrushStyleDialog(QColor color, Qt::BrushStyle initialBrushstyle,QWidget *parent = Q_NULLPTR);
protected:
    void setPattern(QColor color,Qt::BrushStyle initialBrushstyle);
};

#endif // RECTANGLEBRUSHSTYLEDIALOG_H
