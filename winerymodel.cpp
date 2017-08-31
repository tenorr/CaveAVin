#include "winerymodel.h"
#include <QDebug>

WineryModel::WineryModel(QObject *parent, QSqlDatabase db)
    :AbstractWineTableModel("Winery",parent,db)
{
    setRelation(fieldIndex("Country"),QSqlRelation("Country","Id",tr("Country")));
}


void WineryModel::selectRecords(const QString &text, const int countryId)
{
    QString str = (text.isEmpty())? QString() : QString("Winery LIKE '%%1%'").arg(text);
    if (countryId !=0) {
        if (!text.isEmpty())
            str.append(" AND ");
        str.append(QString("Winery.Country = %1").arg(countryId));}
    setFilter(str);
}
