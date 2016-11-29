#-------------------------------------------------
#
# Project created by QtCreator 2016-11-13T20:36:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ProcessMonitor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h

FORMS    += mainwindow.ui

DISTFILES += \
    cpuData \
    memData \
    saidaCPU.txt \
    saidaMem.txt
