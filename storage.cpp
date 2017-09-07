#include "storage.h"
#include  "storagewindow.h"
#include <QDebug>

Storage::Storage(QSqlRecord rec, Cellar * cellar, QGraphicsItem *parent)
    : RectGraphicsObject(rec, parent)
{
    //Set pointer to  Cellar
    setCellar(cellar);

    // Set Storage Type from record
    setStorageType(rec.value("Type").toInt());
}

int Storage::storageType() const
{
    return m_storageType;
}

void Storage::setStorageType(int storageType)
{
    m_storageType = storageType;
}

void Storage::clearBottleChildren()
{
    QList<QGraphicsItem *> bottleItems = childItems();
        foreach (QGraphicsItem * item, bottleItems) {
            if (item->type()==UserType+1) {
                Bottle *bottle = static_cast<Bottle *>(item);
                bottle->setSublocationId(0);
                QPointF bPos = bottle->pos()+pos();
                bottle->setPos(bPos);
                bottle->setParentItem(0);
                bottle->bottleModel()->changeStorage(bottle->id(),0);
            }
        }
}

void Storage::clearZoneChildren()
{
    cellar()->zoneModel()->deleteStorageZones(id());
}

Cellar *Storage::cellar() const
{
    return m_cellar;
}

void Storage::setCellar(Cellar *cellar)
{
    m_cellar = cellar;
}

void Storage::changeRectangleSize(QPointF dPos)
{
    // Genral procedure
    GraphicsObject::changeRectangleSize(dPos);

    // Calculate proportional ratios
    QPointF ratios = QPointF(rect().width()/(rect().width()-dPos.x()),rect().height()/(rect().height()-dPos.y()));

    // Find every bottle in the Storage
    QList<QGraphicsItem *> list = childItems();
    foreach (QGraphicsItem * item, list) {
        if (item->type() == UserType+1) {
//          Apply ratios to bottle position
            Bottle *bottle = static_cast<Bottle *>(item);
            QPointF newPos = QPointF(bottle->pos().x() * ratios.x(),bottle->pos().y() * ratios.y());
            bottle->setPos(newPos);
        }
    }
}

int Storage::type() const
{
    return UserType;
}

void Storage::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event) {

    StorageWindow *w = new StorageWindow(id(),cellar(),static_cast<QWidget *>(cellar()->views()[0]));
    w->setAttribute(Qt::WA_DeleteOnClose,true);
    w->show();
    }
}
