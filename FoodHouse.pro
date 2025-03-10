#-------------------------------------------------
#
# Project created by QtCreator 2014-12-13T21:31:33
#
#-------------------------------------------------

QT       += core gui sql printsupport serialport
CONFIG   += static

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FoodHouse
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    printwindow.cpp \
    addwindow.cpp \
    settings.cpp \
    changepassworddialog.cpp \
    logindialog.cpp \
    profilewindow.cpp \
    reportwindow.cpp \
    printpreview.cpp

HEADERS  += mainwindow.h \
    printwindow.h \
    addwindow.h \
    settings.h \
    changepassworddialog.h \
    logindialog.h \
    profilewindow.h \
    reportwindow.h \
    printpreview.h

FORMS    += mainwindow.ui \
    printwindow.ui \
    addwindow.ui \
    changepassworddialog.ui \
    logindialog.ui \
    profilewindow.ui \
    reportwindow.ui
