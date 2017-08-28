#ifndef WINETYPEVIEW_H
#define WINETYPEVIEW_H

#include "circlebrushstyledialog.h"
#include "winetypeevent.h"

#include <QSqlTableModel>
#include <QSqlRecord>
#include <QTableView>
#include <QContextMenuEvent>
#include <QMenu>
#include <QColorDialog>


class WineTypeView : public QTableView
{
Q_OBJECT
public:
    WineTypeView(QSqlTableModel *model, QWidget *parent = Q_NULLPTR);

    QColor selectedColor() const;
    void setSelectedColor(const QColor &selectedColor);

    Qt::BrushStyle selectedBrushStyle() const;
    void setSelectedBrushStyle(const Qt::BrushStyle &selectedBrushStyle);

    int selectedRow() const;
    void setSelectedRow(int selectedRow);

    QSqlTableModel *SqlModel();
    void setSqlModel(QSqlTableModel *SqlModel);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void changeColor();
    void changeBrushStyle();

private:
    void sendWineTypeEvent(int wineTypeId);

private:
    int m_selectedRow;
    QColor m_selectedColor;
    Qt::BrushStyle m_selectedBrushStyle;
    QSqlTableModel * m_SqlModel;
};

#endif // WINETYPEVIEW_H
