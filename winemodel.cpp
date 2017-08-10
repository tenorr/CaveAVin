#include "winemodel.h"
#include <QDebug>

WineModel::WineModel(QObject *parent, QSqlDatabase db)
:AbstractWineTableModel("Wine",parent,db)
{
    setRelation(fieldIndex("Domaine"),QSqlRelation("Domaine","Id","Domaine"));
    setRelation(fieldIndex("Type"),QSqlRelation("Wine_Type","Id","Type"));
}

void WineModel::selectRecords(const QString &text1, const QString &text2, const int typeId)
{
    // Find domaine Ids in range
    QVector<int> domaineId;
    QSqlQuery query;
    QString inText;
    if (!text1.isEmpty()) {
    query.prepare(QString("SELECT Id FROM Domaine WHERE Domaine LIKE '%%1%'").arg(text1));
    query.exec();
    while (query.next()) {
        domaineId << query.value(0).toInt();
    }
    // Prepare the IN clause

    if (domaineId.size()>0) {
        inText ="Wine.Domaine IN (";
        foreach (int i, domaineId) {
            inText.append(QString::number(i));
            inText.append(',');
        }
        // Delete the last coma
        inText = inText.left(inText.size()-1);
        inText.append(')');
        }}

    QString str  = (text1.isEmpty())? QString() : inText;

    if(!text2.isEmpty()) {
        if(!inText.isEmpty())
            str.append(" AND ");
        str.append(QString("Wine LIKE '%%1%'").arg(text2));
    }
    if (typeId !=0) {
        if ((!inText.isEmpty()) || (!text2.isEmpty()))
            str.append(" AND ");
        str.append(QString("Wine.Type = %1").arg(typeId));}
    setFilter(str);
}
