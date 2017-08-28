#ifndef WINETYPEEVENT_H
#define WINETYPEEVENT_H

#include <QEvent>
#include <QApplication>
#include <QWidget>
#include <QGraphicsView>

class WineTypeEvent : public QEvent
{
public:
    WineTypeEvent(int wineTypeId, int wineId=0);

    int wineTypeId() const;
    void setWineTypeId(int wineTypeId);

    int wineId() const;
    void setWineId(int wineId);

    void sendEvent();

private:
    int m_wineTypeId;
    int m_wineId;

};

#endif // WINETYPEEVENT_H
