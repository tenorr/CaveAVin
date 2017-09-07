#ifndef BOTTLE_H
#define BOTTLE_H

#include "abstractbottle.h"
#include "cellar.h"

#include <QSqlRecord>

class Cellar;

class Bottle : public AbstractBottle
{
Q_OBJECT
public:
    Bottle(QSqlRecord rec, Cellar *cellar, QGraphicsItem *parent = Q_NULLPTR);

    int type() const; // UserType+1;

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    void changeStorage(int newStorageId);

};

#endif // BOTTLE_H
