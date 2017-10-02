#include "wineprizemodel.h"
#include <QDate>
#include <QDebug>

WinePrizeModel::WinePrizeModel(int wineId, QObject *parent, QSqlDatabase db)
    : AbstractWineTableModel("WinePrize",parent, db)
{
    // Set Model, Strategy and relations
    setRelation(3, QSqlRelation("Contest", "Id", "Contest"));
    setRelation(4, QSqlRelation("Prize", "Id", "Prize"));

    // Populate fitering by wine Id
    setFilter(QString("WineId = %1").arg(wineId));
    select();

    // Set Meta Object to connect signals
    setWineId(wineId);
    setObjectName("winePrizeModel");
    QMetaObject::connectSlotsByName(this);
}

void WinePrizeModel::on_winePrizeModel_primeInsert(int row, QSqlRecord &record)
{
    record.setNull("Id");
    record.setValue("WineId",wineId());
    record.setGenerated("WineId",true);
    record.setValue("Millesime",QDate::currentDate().year());
    record.setGenerated("Millesime",true);

    Q_UNUSED(row)
}

void WinePrizeModel::on_winePrizeModel_beforeInsert(QSqlRecord &record)
{
    // Clear Record if not properly filled
    if ((record.value("Millesime").toInt() < 1900) || (record.value("Millesime").toInt() > QDate::currentDate().year())
                                                   || (record.value("Contest").isNull()) || (record.value("Prize").isNull()))
        record.clear();
}

void WinePrizeModel::on_winePrizeModel_beforeUpdate(int row, QSqlRecord &record)
{
    // Clear Record if not properly filled
    if ((record.value("Millesime").toInt() < 1900) || (record.value("Millesime").toInt() > QDate::currentDate().year())
                                                   || (record.value("Contest").isNull()) || (record.value("Prize").isNull()))
         record.clear();

    Q_UNUSED(row)
}

int WinePrizeModel::wineId() const
{
    return m_wineId;
}

void WinePrizeModel::setWineId(int wineId)
{
    m_wineId = wineId;
}

