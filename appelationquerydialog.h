#ifndef APPELATIONQUERYDIALOG_H
#define APPELATIONQUERYDIALOG_H

#include "querydialog.h"
#include "appelationmodel.h"



class AppelationQueryDialog : public QueryDialog
{
    Q_OBJECT

public:
    AppelationQueryDialog(QString appelationStr, QSqlDatabase db, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    AppelationModel *appelationModel() const;
    int findRegionId(const QString &text);
    int findAppelationTypeId(const QString &text);

public slots:
    void on_queryLineEdit_textEdited(const QString &text);
    void on_queryComboBox_activated(const QString &text);
    void on_queryComboBox2_activated(const QString &text);
    void on_queryComboBox3_activated(const QString &text);
    int findWineTypeId(const QString &text);

};

#endif // APPELATIONQUERYDIALOG_H
