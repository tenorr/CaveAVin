#include "abstractmodelformdialog.h"

AbstractModelFormDialog::AbstractModelFormDialog(QWidget *parent, Qt::WindowFlags f)
    :AbstractFormDialog(parent,f)
{

}

AbstractWineTableModel *AbstractModelFormDialog::model() const
{
    return m_model;
}

void AbstractModelFormDialog::setModel(AbstractWineTableModel *model)
{
    m_model = model;
}

void AbstractModelFormDialog::populateCombo(const QString &comboName, const QString &fieldName, const QString &tableName, const QString &filter)
{
    // Create Null value in Combo
    QStringList list;
    list << "";

    // Query the whole value set
    QSqlQuery query;
    QString queryStr = QString("SELECT %1 FROM %2").arg(fieldName,tableName);
    if (!filter.isEmpty())
        queryStr.append(" WHERE "+ filter);
    queryStr.append(" ORDER BY Id");
    query.prepare(queryStr);
    query.exec();
    while (query.next()) {
        list << query.value(fieldName).toString();
    }
    combo()[indexOf(comboName)]->addItems(list);
}

void AbstractModelFormDialog::setInitialData(int id)
{
    int index = model()->rowPosition(id);
    QSqlRecord rec = (index ==-1)? model()->record() : model()->record(index);

    // Create new Id number if id=-1 (Create Mode)
    if (index ==-1)
        rec.setValue("Id",model()->newId());

    // Populate variables with record data
      QSqlField field;

      // Retrieve each field and affect to typed variables
      for (int i=0, n=rec.count();i<n;i++) {
          field = rec.field(i);
          WidgetType t = typeOf(field.name());

          switch (t) {
          case LineEdit:
              lineEdit()[indexOf(field.name())]->setText(field.value().toString());
              break;
          case TextEdit:
              textEdit()[indexOf(field.name())]->setText(field.value().toString());
              break;
          case SpinBox:
              spinBox()[indexOf(field.name())]->setValue(field.value().toInt());
              break;
          case DoubleSpinBox:
              doubleSpinBox()[indexOf(field.name())]->setValue(field.value().toDouble());
              break;
          case DateEdit:
              dateEdit()[indexOf(field.name())]->setDate(field.value().toDate());
              break;
          case ComboBox:
              if (combo()[indexOf(field.name())]->count() !=0)
                  combo()[indexOf(field.name())]->setCurrentIndex(field.value().toInt());
              break;
          default:
              break;
          }
}

}
