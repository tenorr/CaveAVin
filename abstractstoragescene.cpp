#include "abstractstoragescene.h"
#include "storagetablemodel.h"

AbstractStorageScene::AbstractStorageScene(int id, QObject *parent)
    :AbstractScene (id,parent) // Parent is set to the Storage Delegate Object
{
    // Set SQL Tables
    setDb(delegate()->db());
    setBottleModel(delegate()->bottleModel());
    setZoneModel(delegate()->zoneModel());
    setTableName("Storage");
    setCellarId(delegate()->cellarId());

    setTableRatio();
}

StorageDelegate *AbstractStorageScene::delegate()
{
    return static_cast<StorageDelegate *>(parent());
}

QPointF AbstractStorageScene::ratios()
{
    return delegate()->ratios();
}

void AbstractStorageScene::setRatios(const QPointF &ratios)
{
    delegate()->setRatios(ratios);
}

int AbstractStorageScene::cellarId() const
{
    return m_cellarId;
}

void AbstractStorageScene::setCellarId(int cellarId)
{
    m_cellarId = cellarId;
}

QColor AbstractStorageScene::color()
{
    // Find color in dataBase
    QSqlQuery query;
    query.prepare(QString("SELECT Red, Green, Blue FROM Storage WHERE Id = %1").arg(id()));
    query.exec();
    if (query.first())
        return QColor(query.value("Red").toInt(),query.value("Green").toInt(),query.value("Blue").toInt());

    return QColor(Qt::black); // returned in case of database error
}

Qt::BrushStyle AbstractStorageScene::brushStyle()
{
    // Find Style in dataBase
    QSqlQuery query;
    query.prepare(QString("SELECT BrushStyle FROM Storage WHERE Id = %1").arg(id()));
    query.exec();
    if (query.first())
        return Qt::BrushStyle(query.value("BrushStyle").toInt());

    return Qt::NoBrush; // returned in case of database error
}

void AbstractStorageScene::setTableRatio()
{
    // Prepare Query to retrieve width and height
    QSqlQuery query;
    query.prepare(QString("SELECT Width, Height, QRect FROM Storage WHERE Id = %1").arg(id()));
    query.exec();
    if (query.first() ) {

    QRect rect = StorageTableModel::decodeToQRect(query.value("QRect").toString(),true);
    setSceneRect(rect);

    qreal xRatio = width()/query.value(0).toInt();
    qreal yRatio = height()/query.value(1).toInt();

    // Set Ratio in delegate
    setRatios(QPointF(xRatio,yRatio));

    // Store ratios in SQL Table
    query.prepare(QString("UPDATE Storage SET XRatio = :XRatio , YRatio = :YRatio WHERE Id = :id"));
    query.bindValue(":id",id());
    query.bindValue(":XRatio",xRatio);
    query.bindValue(":YRatio",yRatio);
    query.exec();
    }
}

void AbstractStorageScene::solveZeroPositionIssue(StorageBottle *bottle)
{
     // WARNING don't take into account the width of the scene
    QPointF bottlePos = bottleModel()->storagePosition(bottle->id());
    // Test if position at (0,0)
    if (bottlePos.isNull()) {
        bottlePos = firstAvailableZeroPosition();
        bottleModel()->setStoragePosition(bottle->id(), QPointF(bottlePos.x(),15));
        bottle->setPos(bottlePos);
    }
}

QPointF AbstractStorageScene::firstAvailableZeroPosition()
{
     // WARNING don't take into account the width of the scene
     // Find the first available position with no Graphics item
    QPointF bottlePos = QPointF(40,15);
    while (items(bottlePos).size() >1)
        bottlePos = bottlePos + QPointF(20,0);
    return bottlePos;
}

