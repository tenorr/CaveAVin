#include "brushstyledialog.h"

BrushStyleDialog::BrushStyleDialog(QWidget *parent)
    :QDialog(parent)
{
    // Create Title
    setWindowTitle(tr("Change BrushStyle"));

    // Create Scene for Rectangles and view
    setScene(new QGraphicsScene());
    setGraphicsView(new QGraphicsView(scene()));

    // Create Dialog Buttons
    setDialogButtonBox(new QDialogButtonBox(QDialogButtonBox::Ok
                                          | QDialogButtonBox::Cancel));
    // Set Layout
    setBoxLayout(new QBoxLayout(QBoxLayout::TopToBottom));
}

QDialogButtonBox *BrushStyleDialog::dialogButtonBox() const
{
    return m_dialogButtonBox;
}

void BrushStyleDialog::setDialogButtonBox(QDialogButtonBox *dialogButtonBox)
{
    connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    m_dialogButtonBox = dialogButtonBox;
}

QBoxLayout *BrushStyleDialog::boxLayout() const
{
    return m_boxLayout;
}

void BrushStyleDialog::setBoxLayout(QBoxLayout *boxLayout)
{
    boxLayout->addWidget(graphicsView());
    boxLayout->addWidget(dialogButtonBox());
    setLayout(boxLayout);
    m_boxLayout = boxLayout;
}

QGraphicsScene *BrushStyleDialog::scene() const
{
    return m_scene;
}

void BrushStyleDialog::setScene(QGraphicsScene *scene)
{
    scene->setSceneRect(0,0,(RECTSIZE+10)*3+10,(RECTSIZE+10)*5+10);
    m_scene = scene;
}

QGraphicsView *BrushStyleDialog::graphicsView() const
{
    return m_graphicsView;
}

void BrushStyleDialog::setGraphicsView(QGraphicsView *graphicsView)
{
    m_graphicsView = graphicsView;
}

Qt::BrushStyle BrushStyleDialog::selectedStyle()
{
   QList<QGraphicsItem *> items = scene()->items();
   foreach (QGraphicsItem * item, items) {
       if (item->isSelected()) {
           QAbstractGraphicsShapeItem *shapeItem = static_cast<QAbstractGraphicsShapeItem *>(item);
           return shapeItem->brush().style();
       }
   }
   return Qt::NoBrush;
}
