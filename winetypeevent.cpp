#include "winetypeevent.h"

WineTypeEvent::WineTypeEvent(int wineTypeId, int wineId)
:QEvent(QEvent::User)
{
    setWineTypeId(wineTypeId);
    setWineId(wineId);
}

int WineTypeEvent::wineTypeId() const
{
    return m_wineTypeId;
}

void WineTypeEvent::setWineTypeId(int wineTypeId)
{
    m_wineTypeId = wineTypeId;
}

int WineTypeEvent::wineId() const
{
    return m_wineId;
}

void WineTypeEvent::setWineId(int wineId)
{
    m_wineId = wineId;
}

void WineTypeEvent::sendEvent()
{
    // Send the event to every QGraphicsScene of the Top Level widgets

    // Find TopLevel Widgets
    QWidgetList list = qApp->topLevelWidgets();
    // Find children of class GraphicsScreen and sent event to their scene;
    foreach (QWidget *w, list) {
        QGraphicsView * view = w->findChild<QGraphicsView *>();
        if (view)
            QApplication::sendEvent(view->scene(),this);
    }
}
