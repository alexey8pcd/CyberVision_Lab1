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
    convolution.cpp \
    imageutil.cpp \
    pyramid.cpp \
    kernel.cpp \
    fimage.cpp \
    interestpointsdetector.cpp \
    histogram.cpp \
    descriptor.cpp \
    descriptorsearcher.cpp

HEADERS  += mainwindow.h \
    imageutil.h \
    convolution.h \
    pyramid.h \
    kernel.h \
    fimage.h \
    interestpointsdetector.h \
    datatypes.h \
    histogram.h \
    descriptor.h \
    descriptorsearcher.h

FORMS    += mainwindow.ui
