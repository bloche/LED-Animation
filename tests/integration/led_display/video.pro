#Project file created by Grant Boomer. Hopefully it works

QT       += core
QT       += network

QT       -= gui

TARGET = video
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += debug

TEMPLATE = app


SOURCES += main.cpp \
    ledmatrixdriver.cpp \
    leddriver.cpp \
    videocontroller.cpp \
    videodisplay.cpp \
    videoreader.cpp

HEADERS += \
    leddriver.h \
    ledmatrixdriver.h \
    videocontroller.h \
    videodisplay.h \
    videoreader.h 

QMAKE_CXXFLAGS += -std=c++0x `pkg-config opencv --cflags --libs`

#DEFINES += USE_STD_OUT

unix:!macx|win32: LIBS += -L$$PWD/../build-RPLDCommunications/ -lRPLDCommunications
LIBS += -lwiringPi

INCLUDEPATH += $$PWD/../rpldcommunications
DEPENDPATH += $$PWD/../rpldcommunications
