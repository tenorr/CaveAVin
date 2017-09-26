#ifndef MATRIXSCENE_H
#define MATRIXSCENE_H

#include "abstractstoragescene.h"


class MatrixScene : public AbstractStorageScene
{
    Q_OBJECT
public:
    MatrixScene(int storageId, QObject *parent = Q_NULLPTR);

protected:
     virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent);

private:
    bool isCircular();
    bool isDisabledItem(int rackElement);
    void positionBottleOnRackElement(StorageBottle *bottle);

    MatrixRackElement * rackElement(int number);

public slots:
    void manageRackElement(int bottleId, int newRackElement);

private slots:
    void setElementEnabled();
    void changeElementShape();
    void addBottle();
    void changeBottleData();
    void drinkBottle();
};

#endif // MATRIXSCENE_H
