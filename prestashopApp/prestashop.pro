#-------------------------------------------------
#
# Project created by QtCreator 2011-09-15T12:30:17
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += xml
TARGET = prestashop
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialogsettings.cpp \
    prestaconnector.cpp \
    trackingnumberdialog.cpp \
    senddialog.cpp

HEADERS  += mainwindow.h \
    dialogsettings.h \
    prestaconnector.h \
    trackingnumberdialog.h \
    senddialog.h

FORMS    += mainwindow.ui \
    dialogsettings.ui \
    trackingnumberdialog.ui \
    senddialog.ui

RESOURCES += \
    resources.qrc
