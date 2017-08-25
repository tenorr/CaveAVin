#-------------------------------------------------
#
# Project created by QtCreator 2017-06-25T21:06:47
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets uitools

TARGET = WineCave
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        winecavemainwindow.cpp \
    room.cpp \
    graphicstext.cpp \
    graphicstextdialog.cpp \
    container.cpp \
    containertablemodel.cpp \
    brushstyledialog.cpp \
    winetypedialog.cpp \
    winetypeview.cpp \
    rectanglebrushstyledialog.cpp \
    circlebrushstyledialog.cpp \
    winetypedelegate.cpp \
    bottletablemodel.cpp \
    bottledialog.cpp \
    abstractwinetablemodel.cpp \
    graphicsobject.cpp \
    bottle.cpp \
    containerwindow.cpp \
    containerscene.cpp \
    abstractscene.cpp \
    rectgraphicsobject.cpp \
    zone.cpp \
    abstractrectmodel.cpp \
    zonetablemodel.cpp \
    abstractbottle.cpp \
    containerbottle.cpp \
    domainequerydialog.cpp \
    domainemodel.cpp \
    querydialog.cpp \
    domainedialog.cpp \
    abstractformdialog.cpp \
    winequerydialog.cpp \
    winemodel.cpp \
    winedialog.cpp \
    abstractmodelformdialog.cpp \
    regionquerydialog.cpp \
    regionmodel.cpp \
    appelationquerydialog.cpp \
    appelationmodel.cpp \
    grapevarietyselectiondialog.cpp

HEADERS += \
        winecavemainwindow.h \
    room.h \
    graphicstext.h \
    graphicstextdialog.h \
    container.h \
    containertablemodel.h \
    brushstyledialog.h \
    winetypedialog.h \
    winetypeview.h \
    rectanglebrushstyledialog.h \
    circlebrushstyledialog.h \
    winetypedelegate.h \
    bottletablemodel.h \
    bottledialog.h \
    abstractwinetablemodel.h \
    graphicsobject.h \
    bottle.h \
    containerwindow.h \
    containerscene.h \
    abstractscene.h \
    rectgraphicsobject.h \
    zone.h \
    abstractrectmodel.h \
    zonetablemodel.h \
    abstractbottle.h \
    containerbottle.h \
    domainequerydialog.h \
    domainemodel.h \
    querydialog.h \
    domainedialog.h \
    abstractformdialog.h \
    winequerydialog.h \
    winemodel.h \
    winedialog.h \
    abstractmodelformdialog.h \
    regionquerydialog.h \
    regionmodel.h \
    appelationquerydialog.h \
    appelationmodel.h \
    grapevarietyselectiondialog.h

FORMS += \
    bottleDialog.ui \
    queryForm.ui \
    domaineForm.ui \
    wineForm.ui \
    grapevarietyform.ui

RESOURCES += \
    res.qrc
