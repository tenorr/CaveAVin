#include "winetypedelegate.h"

WineTypeDelegate::WineTypeDelegate(QObject *parent)
    :QStyledItemDelegate(parent)
{

}

QWidget *WineTypeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Provide an editor with QLineEdit Widget
        QLineEdit *editor = new QLineEdit(parent);
        return editor;
        Q_UNUSED(option);
        Q_UNUSED(index);
}

void WineTypeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    // Copy data (edit role) of the model to the LineEdit editor
        QString text = index.model()->data(index,Qt::EditRole).toString();
        QLineEdit *lineEdit = static_cast<QLineEdit *>(editor);
        lineEdit->setText(text);
}

void WineTypeDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Manage the editor geometry when the editor is created and when the item size or position in the view is changed
        editor->setGeometry(option.rect);
        Q_UNUSED(index);
}

void WineTypeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    //Submit data to the model
    QLineEdit *lineEdit = static_cast<QLineEdit *>(editor);

    // Retrieve row and model
    int row = index.row();
    QSqlTableModel *SqlModel = static_cast<QSqlTableModel *>(model);

    //Test if the text is not already in the table
    bool fSubmitData = true;
    QString text = lineEdit->text();
    QSqlRecord rec;
    for (int i=0;i<SqlModel->rowCount();i++) {
        if (i!=row) {
            rec = SqlModel->record(i);
            fSubmitData = fSubmitData && (rec.value("Type").toString() != text);
        }
    }

    // Copy the text to the model if allowed
    if (fSubmitData)
        model->setData(index, text, Qt::EditRole);
}
