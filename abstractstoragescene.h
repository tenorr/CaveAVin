#ifndef ABSTRACTSTORAGESCENE_H
#define ABSTRACTSTORAGESCENE_H

#include "abstractscene.h"
class StorageBottle;
#include "storagedelegate.h"

class Cellar;

class AbstractStorageScene : public AbstractScene

{
    Q_OBJECT
public:
    AbstractStorageScene(int id, QObject *parent = Q_NULLPTR);

    StorageDelegate * delegate();

    QPointF ratios();
    void setRatios(const QPointF &ratios);

    int cellarId() const;
    void setCellarId(int cellarId);

protected:
    QColor color();
    Qt::BrushStyle brushStyle();
    void setTableRatio();

private:
    int m_cellarId;
};

#endif // ABSTRACTSTORAGESCENE_H
