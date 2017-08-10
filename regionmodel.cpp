#include "regionmodel.h"

RegionModel::RegionModel(QObject *parent, QSqlDatabase db)
    :AbstractWineTableModel("Region",parent,db)
{
    setRelation(fieldIndex("Country"),QSqlRelation("Country","Id","Country"));
}

void RegionModel::selectRecords(const QString &text, const int regionId)
{
    QString str = (text.isEmpty())? QString() : QString("Region LIKE '%%1%'").arg(text);
    if (regionId !=0) {
        if (!text.isEmpty())
            str.append(" AND ");
        str.append(QString("Region.Country = %1").arg(regionId));}
    setFilter(str);
}
