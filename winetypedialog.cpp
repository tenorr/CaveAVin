#include "winetypedialog.h"
#include <QDebug>

WineTypeDialog::WineTypeDialog(QSqlDatabase db, QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent,f)
{
    // Create Title and minimum width
    setWindowTitle(tr("Define Wine Types"));
    setMinimumWidth(350);

    // Create the Model, the View and the delegate
    setModel(new QSqlTableModel(this,db));
    setView(new WineTypeView(model()));
    setDelegate(new WineTypeDelegate);

    // Create Dialog Buttons
    setDialogButtonBox(new QDialogButtonBox());

    // Set Layout
    setBoxLayout(new QBoxLayout(QBoxLayout::TopToBottom));
}

WineTypeDialog::~WineTypeDialog()
{
}

QDialogButtonBox *WineTypeDialog::dialogButtonBox() const
{
    return m_dialogButtonBox;
}

void WineTypeDialog::setDialogButtonBox(QDialogButtonBox *dialogButtonBox)
{

    //Button box laid out in horizontal
    dialogButtonBox->addButton(QDialogButtonBox::Cancel);
    dialogButtonBox->addButton(tr("Create"),QDialogButtonBox::ActionRole);
    dialogButtonBox->addButton(QDialogButtonBox::Save);

    connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &WineTypeDialog::reject);
    connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &WineTypeDialog::accept);
    connect(dialogButtonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(createNewType(QAbstractButton*)));

    m_dialogButtonBox = dialogButtonBox;
}

QBoxLayout *WineTypeDialog::boxLayout() const
{
    return m_boxLayout;
}

void WineTypeDialog::setBoxLayout(QBoxLayout *boxLayout)
{
    boxLayout->addWidget(view());
    boxLayout->addWidget(dialogButtonBox());
    setLayout(boxLayout);
    m_boxLayout = boxLayout;
}

WineTypeView *WineTypeDialog::view() const
{
    return m_view;
}

void WineTypeDialog::setView(WineTypeView *view)
{
    m_view = view;
}

QSqlTableModel *WineTypeDialog::model() const
{
    return m_model;
}

void WineTypeDialog::setModel(QSqlTableModel *model)
{
    // Select Table
    model->setTable("WineType");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    m_model = model;
}


void WineTypeDialog::createNewType(QAbstractButton *button)
{
    if (button->text() == tr("Create")) {
        // Retrieve last id
        QSqlRecord rec = model()->record(model()->rowCount()-1);
        int newId = rec.value(0).toInt()+1;
        // Set default value to record
        rec = model()->record();
        rec.setValue(0,newId);
        rec.setValue(1,tr("New Type # ")+QString::number(newId));
        for (int i=2;i<6;i++)
            rec.setValue(i,0);
        model()->insertRecord(-1,rec);
    }
}

WineTypeDelegate *WineTypeDialog::delegate() const
{
    return m_delegate;
}

void WineTypeDialog::setDelegate(WineTypeDelegate *delegate)
{
    // Set Delegate to the view
    view()->setItemDelegate(delegate);
    m_delegate = delegate;
}
