#include "domainemodel.h"
#include <QDebug>

DomaineModel::DomaineModel(QObject *parent, QSqlDatabase db)
    :AbstractWineTableModel("Domaine",parent,db)
{
    setRelation(fieldIndex("Country"),QSqlRelation("Country","Id",tr("Country")));
}


void DomaineModel::selectRecords(const QString &text, const int countryId)
{
    QString str = (text.isEmpty())? QString() : QString("Domaine LIKE '%%1%'").arg(text);
    if (countryId !=0) {
        if (!text.isEmpty())
            str.append(" AND ");
        str.append(QString("Domaine.Country = %1").arg(countryId));}
    setFilter(str);
}
