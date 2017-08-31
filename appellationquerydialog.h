#ifndef APPELLATIONQUERYDIALOG_H
#define APPELLATIONQUERYDIALOG_H

#include "querydialog.h"
#include "appellationmodel.h"



class AppellationQueryDialog : public QueryDialog
{
    Q_OBJECT

public:
    AppellationQueryDialog(QString appellationStr, QSqlDatabase db, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    AppellationModel *appellationModel() const;
    int findRegionId(const QString &text);
    int findAppellationTypeId(const QString &text);

public slots:
    void on_queryLineEdit_textEdited(const QString &text);
    void on_queryComboBox_activated(const QString &text);
    void on_queryComboBox2_activated(const QString &text);
    void on_queryComboBox3_activated(const QString &text);
    int findWineTypeId(const QString &text);

};

#endif // APPELLATIONQUERYDIALOG_H

