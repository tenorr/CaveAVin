#ifndef REGIONQUERYFORM_H
#define REGIONQUERYFORM_H

#include "querydialog.h"
#include "regionmodel.h"



class RegionQueryDialog : public QueryDialog
{
    Q_OBJECT
public:
    RegionQueryDialog(QString regionStr, QSqlDatabase db, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    RegionModel * regionModel() const;

public slots:
    void on_queryLineEdit_textEdited(const QString &text);
    void on_comboBox_currentIndexChanged(int index);
    //void doAction(QAbstractButton *button);
};

#endif // REGIONQUERYFORM_H
