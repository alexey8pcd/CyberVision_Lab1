#-------------------------------------------------
#
# Project created by QtCreator 2016-03-07T12:56:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lab1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imagehandler.cpp \
    convolution.cpp \
    imageutil.cpp \
    pyramid.cpp

HEADERS  += mainwindow.h \
    imageutil.h \
    imagehandler.h \
    convolution.h \
    pyramid.h

FORMS    += mainwindow.ui
