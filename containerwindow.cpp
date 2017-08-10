#include "containerwindow.h"

ContainerWindow::ContainerWindow(int containerId, Room *room, QWidget *parent)
: QMainWindow(parent)
{
    QRect g =parent->geometry();
    g.setTopLeft(QPoint(50,50));
    setGeometry(g);
    setWindowModality(Qt::WindowModal);
    setWindowTitle(tr("Container #")+QString::number(containerId));
    setRoom(room);

    // Create Menu
    setMenus(new QMenu *);

     // Create Graphics View and affect to central Widget
    setGraphicsView(new QGraphicsView);

    // Create Container Scene
    setContainerScene(new ContainerScene(containerId,*room));
}

QGraphicsView *ContainerWindow::graphicsView() const
{
    return m_graphicsView;
}

void ContainerWindow::setGraphicsView(QGraphicsView *graphicsView)
{
    // Set View Characteristics
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setFixedSize(1980,1220);

    setCentralWidget(graphicsView);
    m_graphicsView = graphicsView;
}

Room *ContainerWindow::room() const
{
    return m_room;
}

void ContainerWindow::setRoom(Room *room)
{
    m_room = room;
}

QMenu **ContainerWindow::menus() const
{
    return m_menus;
}

void ContainerWindow::setMenus(QMenu **menus)
{
    // Create Menus and Actions
    QStringList menuTitles;
    menuTitles << tr("File");

    for (int i=0;i<menuTitles.length();i++) {
        menus[i]=new QMenu(menuTitles.at(i));
        switch (i) {
        case 0:
             menus[i]->addAction(tr("Close"),this,SLOT(close()),QKeySequence(Qt::CTRL+Qt::Key_Q));
            break;
        default:
            break;
                   }
            // Assign Menu to MenuBar
            menuBar()->addMenu(menus[i]);
      }
    m_menus = menus;
}

ContainerScene *ContainerWindow::containerScene() const
{
    return m_containerScene;
}

void ContainerWindow::setContainerScene(ContainerScene *containerScene)
{
    // Assign Room To Graphics View
    graphicsView()->setScene(containerScene);

    m_containerScene = containerScene;
}
