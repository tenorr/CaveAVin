#ifndef CELLAR_H
#define CELLAR_H

#include "abstractscene.h"
#include "storage.h"
#include "storagetablemodel.h"
#include "bottledialog.h"
#include "bottle.h"

class Bottle;

class Cellar : public AbstractScene
{
    Q_OBJECT
public:
    Cellar(StorageTableModel *storageModel, BottleTableModel *bottleModel, ZoneTableModel *zoneModel, int id=1, QObject *parent = Q_NULLPTR);
    ~Cellar();

    StorageTableModel *StorageModel() const;
    void setStorageModel(StorageTableModel *storageModel);
    void positionBottle(int bottleId,QPointF position);
    void moveBottle(int bottleId, QPointF move);
    void clearBottle(int bottleId);

public slots:
    void createStorage();
    void deleteStorage(int id);
    void createBottle();

private:
    void addStorage(QSqlRecord rec);
    void addBottle(QSqlRecord rec);
    Bottle * findBottle(int bottleId);
    Bottle *createBottleFromTableData(int bottleId);
    void createContextMenu();

private:
    StorageTableModel *m_storageModel;

};

#endif // CELLAR_H
