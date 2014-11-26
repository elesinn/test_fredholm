#-------------------------------------------------
#
# Project created by QtCreator 2014-06-03T10:53:27
#
#-------------------------------------------------

QT       += core gui
CONFIG += qwt
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qwt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    somevalues.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    gradient_descent.h \
    somevalues.h

FORMS    += mainwindow.ui \
    dialog.ui \
    somevalues.ui
