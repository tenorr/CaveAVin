#ifndef GRAPHICSTEXTDIALOG_H
#define GRAPHICSTEXTDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>


class GraphicsTextDialog : public QDialog
{
public:
    GraphicsTextDialog(QString currentName,QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~GraphicsTextDialog();

    QLineEdit *lineEdit() const;
    void setLineEdit(QLineEdit *lineEdit);

    QFormLayout *formLayout() const;
    void setFormLayout(QFormLayout *formLayout);

    QString currentName() const;
    void setCurrentName(const QString &currentName);

    QDialogButtonBox *dialogButtonBox() const;
    void setDialogButtonBox(QDialogButtonBox *dialogButtonBox);

    QBoxLayout *boxLayout() const;
    void setBoxLayout(QBoxLayout *boxLayout);

private:
    QFormLayout * m_formLayout;
    QLineEdit *m_lineEdit;
    QString m_currentName;

    QDialogButtonBox * m_dialogButtonBox;
    QBoxLayout * m_boxLayout;
};

#endif // GRAPHICSTEXTDIALOG_H
