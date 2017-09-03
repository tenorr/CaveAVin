#include "containerwindow.h"
#include <QDebug>

ContainerWindow::ContainerWindow(int containerId, Room *room, QWidget *parent)
: QMainWindow(parent)
{
    setContainerId(containerId);
    setRoom(room);

    // Retrieve geometry from DataBase
    QRect geometryRect = getGeometryFromDatabase(containerId);
    setGeometry(geometryRect);

    setWindowModality(Qt::WindowModal);
    setWindowTitle(tr("Container #")+QString::number(containerId));

    // Create Menu
    setMenus(new QMenu *);

     // Create Graphics View and affect to central Widget
    setGraphicsView(new QGraphicsView);

    // Create Container Scene
    setContainerScene(new ContainerScene(containerId,room));
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
    graphicsView->setFixedSize(width(),height());

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


void ContainerWindow::moveEvent(QMoveEvent *event)
{
    if (event) {
        QSqlQuery query;
        query.prepare(QString("SELECT QRect FROM Container WHERE Id = %1").arg(QString::number(containerId())));
        query.exec();
        if (query.first()) {
            QRect rect = decodeToQRect(query.value("QRect").toString());
            rect.setTopLeft(event->pos());
            QString rectStr = encodeFromQRect(rect);
            query.prepare(QString("UPDATE Container "
                                  "SET QRect = '%1' "
                                  "WHERE Id = %2").arg(rectStr).arg(QString::number(containerId())));
            query.exec();
            }
    }
}

QRect ContainerWindow::getGeometryFromDatabase(int containerId)
{
    QSqlQuery query;
    query.prepare(QString("SELECT QRect, Width, Height FROM Container WHERE Id = %1").arg(QString::number(containerId)));
    query.exec();
    if (query.first()) {
       QString rectStr = query.value("QRect").toString();

       // Calculate window Dimensions
       int w = query.value("Width").toInt();
       int h = query.value("Height").toInt();
       // Find minimum ratio
       qreal ratio = qMin(room()->width()/w , room()->height()/h);
        // Store QRect Value
       if (rectStr.isEmpty()) {
           rectStr = encodeFromQRect(QRect(50,50,int(w*ratio), int(h*ratio)));
       }
       else
       {
           QRect rect = decodeToQRect(rectStr);
           rect.setWidth(int(w*ratio));
           rect.setHeight(int(h*ratio));
           rectStr = encodeFromQRect(rect);}
       // Update DataBase
        query.prepare(QString("UPDATE Container "
                              "SET QRect = '%1' "
                              "WHERE Id = %2").arg(rectStr).arg(QString::number(containerId)));
        query.exec();
        return decodeToQRect(rectStr);
         }
    return QRect();
}

QString ContainerWindow::encodeFromQRect(const QRect rect)
{
    QString str("QRect(");
    str.append(QString::number(rect.x()));
    str.append(",");
    str.append(QString::number(rect.y()));
    str.append(" ");
    str.append(QString::number(rect.width()));
    str.append("x");
    str.append(QString::number(rect.height()));
    str.append(")");
    return str;
}

QRect ContainerWindow::decodeToQRect(const QString &text)
{
    QRect rect;
    QString str = text;
    str.remove(0,str.indexOf("(")+1);
    int n = (str.left(str.indexOf(","))).toInt();
    rect.setX(n);
    str.remove(0,str.indexOf(",")+1);
    n = (str.left(str.indexOf(" "))).toInt();
    rect.setY(n);
    str.remove(0,str.indexOf(" ")+1);
    n = (str.left(str.indexOf("x"))).toInt();
    rect.setWidth(n);
    str.remove(0,str.indexOf("x")+1);
    n = (str.left(str.indexOf(")"))).toInt();
    rect.setHeight(n);
    return rect;
}

int ContainerWindow::containerId() const
{
    return m_containerId;
}

void ContainerWindow::setContainerId(int containerId)
{
    m_containerId = containerId;
}
