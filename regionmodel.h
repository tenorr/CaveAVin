#ifndef REGIONMODEL_H
#define REGIONMODEL_H

#include "abstractwinetablemodel.h"



class RegionModel : public AbstractWineTableModel
{
public:
    RegionModel(QObject *parent, QSqlDatabase db);

    void selectRecords(const QString &text, const int regionId);
};

#endif // REGIONMODEL_H
