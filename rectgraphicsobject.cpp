#include "rectgraphicsobject.h"

RectGraphicsObject::RectGraphicsObject(QSqlRecord rec, QGraphicsItem *parent)
    : GraphicsObject(parent)
{
    // Set Z coordinate to be on the background
    setZValue(0);

    // Set Data from record
    setId(rec.value("Id").toInt());
    setRect(QRect(rec.value("XPos").toInt(),rec.value("YPos").toInt(),rec.value("Width").toInt(),rec.value("Height").toInt()));
    setColor(QColor(rec.value("Red").toInt(),rec.value("Green").toInt(),rec.value("Blue").toInt()));
    setBrushStyle((Qt::BrushStyle) rec.value("BrushStyle").toInt());

    // Set initial position
    setPos(rect().x(),rect().y());
}

QRectF RectGraphicsObject::boundingRect() const
{
    // Include half the pen to paint an outline
    qreal penWidth = 1;

    // Return an estimate of the area painted by the item
    return QRectF(- penWidth / 2, - penWidth / 2, rect().width()+ penWidth, rect().height() + penWidth);
}

void RectGraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(QBrush(color(),brushStyle()));
    painter->drawRect(QRect(0,0,rect().width(),rect().height()));
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void RectGraphicsObject::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    // Create Menu and execute selected item
    QMenu menu;
    menu.addAction(tr("Change Color"),this,SLOT(changeColor()));
    menu.addAction(tr("Change Style"),this,SLOT(changeBrushStyle()));
    menu.exec(event->screenPos());
}

void RectGraphicsObject::keyPressEvent(QKeyEvent *event)
{
    if (event) {
        if (event->key() == Qt::Key_Delete) {
            QMessageBox msgBox;
              msgBox.setText(tr("The item is about to be deleted."));
              msgBox.setInformativeText(tr("Do you want to delete?"));
              msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
              msgBox.setDefaultButton(QMessageBox::Ok);
              if (msgBox.exec() == QMessageBox::Ok) {
            clearChildren();
            emit itemToBeDeleted(id());}
        }
        else
            QGraphicsObject::keyPressEvent(event);
    }
}

void RectGraphicsObject::clearChildren()
{
    clearBottleChildren();
    clearZoneChildren();
}

void RectGraphicsObject::clearBottleChildren()
{

}

void RectGraphicsObject::clearZoneChildren()
{

}

void RectGraphicsObject::changeColor()
{
    // Display colorDialog
    QColor newColor = QColorDialog::getColor(color());

    if (newColor.isValid()) {
        // Change Color and emit signal
      if (newColor != color()) {
         setColor(newColor);
         emit colorChanged(newColor, id());
         }
    }
}

void RectGraphicsObject::changeBrushStyle()
{
    RectangleBrushStyleDialog* dialog = new RectangleBrushStyleDialog(color(),brushStyle());
    if (dialog->exec() == QDialog::Accepted) {
        // Update Brush Style and emit signal
        Qt::BrushStyle bs = dialog->selectedStyle();
        setBrushStyle(bs);
        emit brushStyleChanged(bs,id());
        }
    delete dialog;
}
