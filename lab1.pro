#-------------------------------------------------
#
# Project created by QtCreator 2016-03-07T12:56:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++14
CONFIG += c++14
TARGET = lab1
TEMPLATE = app



SOURCES += main.cpp\
        mainwindow.cpp \
    convolution.cpp \
    imageutil.cpp \
    kernel.cpp \
    fimage.cpp \
    interestpointsdetector.cpp \
    siftdescriptorssearcher.cpp \
    descriptorsmatcher.cpp \
    pyramidbuilder.cpp

HEADERS  += mainwindow.h \
    imageutil.h \
    convolution.h \
    kernel.h \
    fimage.h \
    interestpointsdetector.h \
    datatypes.h \
    descriptor.h \
    siftdescriptorssearcher.h \
    descriptorsmatcher.h \
    pyramidbuilder.h

FORMS    += mainwindow.ui
