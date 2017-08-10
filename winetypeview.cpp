#include "winetypeview.h"

WineTypeView::WineTypeView(QSqlTableModel *model, QWidget *parent)
    :QTableView(parent)
{
    setSqlModel(model);

    setModel(SqlModel());

    // Show only first column
    for (int i=0;i<6;i++)
        if (i!=1)
            hideColumn(i);

    // Ajust column Size
    setColumnWidth(1,300);
}

void WineTypeView::contextMenuEvent(QContextMenuEvent *event)
{
    if (event) {
    // Retrieve item and save color and brushstyle
        setSelectedRow(rowAt(event->pos().y()));
        QSqlRecord rec = SqlModel()->record(selectedRow());
        setSelectedColor(QColor(rec.value(2).toInt(),rec.value(3).toInt(),rec.value(4).toInt()));
        setSelectedBrushStyle((Qt::BrushStyle) rec.value(5).toInt());

    // Create Menu and execute selected item
    QMenu menu;
    menu.addAction(tr("Change Color"),this,SLOT(changeColor()));
    menu.addAction(tr("Change Style"),this,SLOT(changeBrushStyle()));
    menu.exec(event->globalPos());
    }
}

void WineTypeView::changeColor()
{
    // Display colorDialog
    QColor newColor = QColorDialog::getColor(selectedColor());

    if (newColor.isValid()) {
        // Change Color
        if (newColor != selectedColor()) {
            // Set the new color in the DataBase
           QSqlRecord rec = SqlModel()->record(selectedRow());
           int id = rec.value("Id").toInt();
           rec.setValue(2,newColor.red());
           rec.setValue(3,newColor.green());
           rec.setValue(4,newColor.blue());
           SqlModel()->setRecord(selectedRow(),rec);
           SqlModel()->submitAll();
           emit colorChanged(id,newColor);
         }
    }
}

void WineTypeView::changeBrushStyle()
{
    CircleBrushStyleDialog* dialog = new CircleBrushStyleDialog(selectedColor(),selectedBrushStyle());
    if (dialog->exec() == QDialog::Accepted) {
        // Update Brush Style and emit signal
        Qt::BrushStyle bs = dialog->selectedStyle();
        if (bs !=selectedBrushStyle()) {
            QSqlRecord rec = SqlModel()->record(selectedRow());
            int id = rec.value("Id").toInt();
            rec.setValue("BrushStyle",(int) bs);
            SqlModel()->setRecord(selectedRow(),rec);
            SqlModel()->submitAll();
            emit brushStyleChanged(id, bs);
            }
        }
    delete dialog;
}

QSqlTableModel *WineTypeView::SqlModel()
{
    return m_SqlModel;
}

void WineTypeView::setSqlModel(QSqlTableModel *SqlModel)
{
    SqlModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_SqlModel = SqlModel;
}

int WineTypeView::selectedRow() const
{
    return m_selectedRow;
}

void WineTypeView::setSelectedRow(int selectedRow)
{
    m_selectedRow = selectedRow;
}

Qt::BrushStyle WineTypeView::selectedBrushStyle() const
{
    return m_selectedBrushStyle;
}

void WineTypeView::setSelectedBrushStyle(const Qt::BrushStyle &selected_brushStyle)
{
    m_selectedBrushStyle = selected_brushStyle;
}

QColor WineTypeView::selectedColor() const
{
    return m_selectedColor;
}

void WineTypeView::setSelectedColor(const QColor &selectedColor)
{
    m_selectedColor = selectedColor;
}
