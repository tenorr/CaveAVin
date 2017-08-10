#ifndef DOMAINEMODEL_H
#define DOMAINEMODEL_H

#include "abstractwinetablemodel.h"



class DomaineModel : public AbstractWineTableModel
{
public:
    DomaineModel(QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());

    void selectRecords(QString const &text, const int countryId=0);
};

#endif // DOMAINEMODEL_H
