#ifndef ABSTRACTFORMDIALOG_H
#define ABSTRACTFORMDIALOG_H

#include <QDialog>
#include <QFile>
#include <QUiLoader>
#include <QBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QSpinBox>
#include <QDateEdit>
#include <QPushButton>
#include <QTableWidget>


class AbstractFormDialog : public QDialog
{
public:
    AbstractFormDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    QList<QLineEdit *> lineEdit() const;
    void setLineEdit();

    QList<QTextEdit *> textEdit() const;
    void setTextEdit();

    QList<QComboBox *> combo() const;
    void setCombo();

    QDialogButtonBox *buttonBox() const;
    void setButtonBox();

    QList<QSpinBox *> spinBox() const;
    void setSpinBox();

    QList<QDoubleSpinBox *> doubleSpinBox() const;
    void setDoubleSpinBox();

    QList<QDateEdit *> dateEdit() const;
    void setDateEdit();

    QList<QTableWidget *> tableWidget() const;
    void setTableWidget();

    int getId();

protected:
    enum WidgetType {Unknown,LineEdit, TextEdit, SpinBox, DoubleSpinBox, DateEdit, ComboBox, TableWidget};

protected:
    void setForm(const QString &fileName);
    int indexOf(const QString &text);
    WidgetType typeOf(const QString &text);

protected:
    QList<QLineEdit *> m_lineEdit;
    QList<QTextEdit *> m_textEdit;
    QList<QComboBox *> m_combo;
    QList<QSpinBox *> m_spinBox;
    QList<QDoubleSpinBox *> m_doubleSpinBox;
    QList<QDateEdit *> m_dateEdit;
    QList<QTableWidget *> m_tableWidget;
    QDialogButtonBox * m_buttonBox;

};

#endif // ABSTRACTFORMDIALOG_H
