#ifndef WINETYPEVIEW_H
#define WINETYPEVIEW_H

#include <QSqlTableModel>
#include <QSqlRecord>
#include <QTableView>
#include <QContextMenuEvent>
#include <QMenu>
#include <QColorDialog>
#include "circlebrushstyledialog.h"

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

signals:
   void colorChanged(int id, QColor color);
   void brushStyleChanged(int id, Qt::BrushStyle bs);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void changeColor();
    void changeBrushStyle();

private:
    int m_selectedRow;
    QColor m_selectedColor;
    Qt::BrushStyle m_selectedBrushStyle;
    QSqlTableModel * m_SqlModel;
};

#endif // WINETYPEVIEW_H
