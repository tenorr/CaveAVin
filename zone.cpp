#include "zone.h"

Zone::Zone(QSqlRecord rec, QGraphicsItem *parent)
   : RectGraphicsObject(rec, parent)
{

}

int Zone::type() const
{
    return UserType+2;
}
