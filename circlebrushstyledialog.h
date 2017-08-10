#ifndef CIRCLEBRUSHSTYLEDIALOG_H
#define CIRCLEBRUSHSTYLEDIALOG_H

#include "brushstyledialog.h"



class CircleBrushStyleDialog : public BrushStyleDialog
{
public:
    CircleBrushStyleDialog(QColor color, Qt::BrushStyle initialBrushstyle,QWidget *parent = Q_NULLPTR);

protected:
    void setPattern(QColor color,Qt::BrushStyle initialBrushstyle);
};

#endif // CIRCLEBRUSHSTYLEDIALOG_H
