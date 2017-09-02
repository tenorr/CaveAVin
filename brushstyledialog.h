#ifndef BRUSHSTYLEDIALOG_H
#define BRUSHSTYLEDIALOG_H

#include <QBoxLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QAbstractGraphicsShapeItem>

#define RECTSIZE 120

class BrushStyleDialog : public QDialog
{
    Q_OBJECT
public:
    BrushStyleDialog(QWidget *parent = Q_NULLPTR);

    QDialogButtonBox *dialogButtonBox() const;
    void setDialogButtonBox(QDialogButtonBox *dialogButtonBox);

    QBoxLayout *boxLayout() const;
    void setBoxLayout(QBoxLayout *boxLayout);

    QGraphicsScene *scene() const;
    void setScene(QGraphicsScene *scene);

    QGraphicsView *graphicsView() const;
    void setGraphicsView(QGraphicsView *graphicsView);

    Qt::BrushStyle selectedStyle();

protected:
    virtual void setPattern(QColor color,Qt::BrushStyle initialBrushstyle) =0;

private:
    QDialogButtonBox * m_dialogButtonBox;
    QBoxLayout * m_boxLayout;
    QGraphicsScene *m_scene;
    QGraphicsView *m_graphicsView;
};

#endif // BRUSHSTYLEDIALOG_H
