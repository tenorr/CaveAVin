#ifndef WINEMODEL_H
#define WINEMODEL_H

#include "abstractwinetablemodel.h"



class WineModel : public AbstractWineTableModel
{
public:
    WineModel(QObject *parent, QSqlDatabase db);

    void selectRecords(const QString &text1, const QString &text2=QString(), const int typeId=0);
};

#endif // WINEMODEL_H
