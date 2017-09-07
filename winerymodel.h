#ifndef WINERYMODEL_H
#define WINERYMODEL_H

#include "abstractwinetablemodel.h"



class WineryModel : public AbstractWineTableModel
{
public:
    WineryModel(QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());

    void selectRecords(QString const &text, const int countryId=0);
};

#endif // WINERYMODEL_H
