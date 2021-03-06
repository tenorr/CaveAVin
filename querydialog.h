#ifndef QUERYDIALOG_H
#define QUERYDIALOG_H

#include "abstractwinetablemodel.h"

#include <QUiLoader>
#include <QFile>
#include <QBoxLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QSqlQuery>
#include <QHeaderView>
#include <QScrollBar>
#include <QTableView>


class QueryDialog : public QDialog
{
    Q_OBJECT
public:
    QueryDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    QTableView *view() const;

    AbstractWineTableModel *model() const;
    void setModelAndView(AbstractWineTableModel *model);

    int selectedRow();
    QString selectedName();
    int selectedId();
    int selectedIntField(QString fieldName);
    QString selectedStringField(QString fieldName);

    QString selectedFieldName() const;
    void setSelectedFieldName(const QString &selectedFieldName);

    QStringList shownFieldNames() const;
    void setShownFieldNames(const QStringList &shownFieldNames);

    void setView(QTableView *view);

    QList<QComboBox *> queryCombo() const;
    void setQueryCombo();

    QList<QLineEdit *> queryLineEdit() const;
    void setQueryLineEdit();

    QList<QLabel *> queryLabel() const;
    void setQueryLabel();

    QDialogButtonBox *dialogButtonBox() const;
    void setDialogButtonBox(QDialogButtonBox *dialogButtonBox);

protected:
    void setForm();
    void setColumnWidth(QString fieldName, int width);
    void populateCombo(const QString &fieldName, const QString &tableName, int comboIndex=0);
    void selectRowId(int id);
    void setLabelText(const QStringList &textList);
    void setEnabledButtons(bool fEnabled);
    void hideButton(QString buttonName);
    void adjustViewSize();

private:
    void hideQueryRow(int index);
    void moveRow(int index, int lag);
    bool rowIsHidden(int index);

public slots:
    void onViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void onViewModelAboutToBeReset();


protected:
    QList<QLineEdit *> m_queryLineEdit;
    QList<QLabel *> m_queryLabel;
    QList<QComboBox *> m_queryCombo;
    QTableView * m_view;
    AbstractWineTableModel *m_model;
    QString m_selectedFieldName;
    QStringList m_shownFieldNames;
    QDialogButtonBox * m_dialogButtonBox;

private:
    int formWidth;
    int viewWidth;
    int viewXPos;
    int sectionsStart = 25;
};

#endif // QUERYDIALOG_H
