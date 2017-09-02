#include "containerbottle.h"
#include "zone.h"
#include <QDebug>

Zone::Zone(QSqlRecord rec, QGraphicsItem *parent)
   : RectGraphicsObject(rec, parent)
{

}

int Zone::type() const
{
    return UserType+2;
}

void Zone::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // Emit signal for bottles in the zone when moving

    if (event) {
        // Intercept If no shift modifier only
         if (!event->modifiers().testFlag(Qt::ShiftModifier)) {
          // Retrieve move distance
             QPointF delta = event->scenePos() - event->buttonDownScenePos(Qt::LeftButton);
             // If mouse has moved
             if (delta.toPoint() != QPoint(0,0)) {
                // Find ChildItem of Type Container Bottle
                QList<QGraphicsItem *> bottleList = childItems();
                foreach (QGraphicsItem * item, bottleList) {
                    if (item->type() == UserType+3) {
                        ContainerBottle * bottle = static_cast<ContainerBottle *>(item);
                        emit bottlePositionChanged(bottle->id(),delta);
                    }
                }
            }
         }
        // Follow hierachy
        RectGraphicsObject::mouseReleaseEvent(event);
    }
}
