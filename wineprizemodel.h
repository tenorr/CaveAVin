#ifndef WINEPRIZEMODEL_H
#define WINEPRIZEMODEL_H

#include "abstractwinetablemodel.h"
#include <QSqlField>


class WinePrizeModel : public AbstractWineTableModel
{
    Q_OBJECT
public:
    WinePrizeModel(int wineId, QObject *parent, QSqlDatabase db);

    int wineId() const;
    void setWineId(int wineId);

private slots :
    void on_winePrizeModel_primeInsert(int row, QSqlRecord &record);
    void on_winePrizeModel_beforeInsert(QSqlRecord &record);
    void on_winePrizeModel_beforeUpdate(int row, QSqlRecord &record);

private:
    int m_wineId;

};

#endif // WINEPRIZEMODEL_H
