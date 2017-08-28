#ifndef WINETYPEDIALOG_H
#define WINETYPEDIALOG_H

#include "winetypedelegate.h"
#include "winetypeview.h"

#include <QBoxLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QSqlTableModel>
#include <QTableView>
#include <QSqlRecord>

class WineTypeView;

class WineTypeDialog : public QDialog
{
    Q_OBJECT
public:
    WineTypeDialog(QSqlDatabase db , QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~WineTypeDialog();

    QDialogButtonBox *dialogButtonBox() const;
    void setDialogButtonBox(QDialogButtonBox *dialogButtonBox);

    QBoxLayout *boxLayout() const;
    void setBoxLayout(QBoxLayout *boxLayout);

    WineTypeView *view() const;
    void setView(WineTypeView *view);

    QSqlTableModel *model() const;
    void setModel(QSqlTableModel *model);

    WineTypeDelegate *delegate() const;
    void setDelegate(WineTypeDelegate *delegate);

public slots:
    void createNewType(QAbstractButton *button);

private:
    QDialogButtonBox * m_dialogButtonBox;
    QBoxLayout * m_boxLayout;
    WineTypeView * m_view;
    QSqlTableModel *m_model;
    WineTypeDelegate * m_delegate;
};

#endif // WINETYPEDIALOG_H
